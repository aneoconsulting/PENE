import sys
from tokenize import Token
from jinja2 import Environment, FileSystemLoader,PackageLoader
from enum import Enum
import os


class operand:
    def __init__(self):
        self.kind = 'None'
        self.width = 0

class eff_operand():
    def __init__(self,ind,kind):
        self.index=ind
        self.kind=kind


class precision:
    def __init__(self,prec_type):
        self.associated_type = prec_type
        if(prec_type=='float'):
            self.nb_bits=32
        elif (prec_type=='double'):
            self.nb_bits=64
        else:
            self.nb_bits=0


class instruction:
    def __init__(self,token):
        self.token = token
        self.opcode = "None"
        self.optype="None"
        self.operands=[]
        self.ins_isa = "None"
        self.ins_precision=precision('None')
        self.simd_option = "None"
        self.nb_operands=0
        self.nb_elements=0
        self.eff_operands=[]
        self.fma_order=[1,2,3]
        self.isfma_avx512=True
    
        
list_operations =['add', 'sub', 'mul', 'div', 'fmadd', 'fnmadd', 'fmsub', 'fnmsub', 'fmaddsub', 'fmsubadd' ]
class Op(Enum):
    add = 'add'
    sub = 'sub'
    mul = 'mul'
    div = 'div'
    fmadd =  'fmadd'
    fnmadd = 'fnmadd'
    fmsub = 'fmsub'
    fnmsub = 'fnmsub'
    fmaddsub = 'fmaddsub'
    fmsubadd = 'fmsubadd'
    #cmp='cmp'
class op_prec(Enum):
    float = 1
    double = 2
class op_simd(Enum):
    scalar = 1
    packed = 2
class operand_options(Enum):
    XMM = 1
    YMM = 2
    ZMM = 3
    MEM = 4
    MASKmskw = 5
    IMM = 6 #for compare operations


# given Pin enum file extracts tokens ending with SS, PS, SD or PD to a list 
def get_tokens_list(file_path):
    list_tokens=[]
    with open(file_path,'r') as f:
        lines_float=f.read()
        for line_float in lines_float.split('\n'):
            if line_float.lstrip().startswith("XED_IFORM_")== True :
                token_float=line_float.lstrip().split('=')[0]
                parts=token_float.split('_')[2:]
                if(parts[0]).endswith("SS") or (parts[0]).endswith("SD") or (parts[0]).endswith("PS") or (parts[0]).endswith("PD"):
                    list_tokens.append(token_float)
    return list_tokens


# given a token returns its instruction set 
def get_isa(token):
    splits= token.split('_')[2:]
    if(not isfma_amd(splits[0])):
        if(isfma(splits[0])):
            isa="fma"
        else:
            if( splits[0].startswith("V") ) :
                if(splits[-1]=="AVX512"):
                    isa="avx512"
                else:
                    isa="avx"
            else:
                isa="sse"
    return isa


# given a token returns the opcode and the type of the operation
def get_opcode_optype(token):
    splits=token.split('_')[2:]
    op_code=splits[0]
    if(op_code.startswith("V") == False and not isfma_amd(op_code)):
        op_type=op_code[0:-2].lower()
    else:
        if(not isfma_amd(op_code)):
            if(not isfma(op_code)):
                op_type=op_code[1:-2].lower()
            else:
                op_type=op_code[1:-5].lower()
        else:
            op_type=""
    return op_code,op_type


def get_precision(opcode):
    if(opcode[-1] == 'S'):
        ins_precision=precision('float')
    elif (opcode[-1] == 'D'):
        ins_precision=precision('double')
    return ins_precision


def check_if_fma_and_avx512(token):
    splits_check= token.split('_')[2:]
    if(isfma(splits_check[0])):
        if(splits_check[-1]=='AVX512'):
            return True
        else:
            return False
    else:
            return False

        
# given the opcode gives if he instruction is simd or scalar
def get_simd(opcode):
    if(opcode[-2] == 'S'):
        simd_option = 'scalar'
    elif (opcode[-2] == 'P'):
        simd_option = 'packed'
    return simd_option


# returns the list of operands in a token (list of strings)
def get_operand_string_list(token):
    splits= token.split('_')[2:]
    operands_list=[]
    if(splits[-1] == "AVX512"):
        operands_list=splits[1:-1]
    else:
        operands_list=splits[1:]
    return operands_list


# get operand width and kind 
def get_operand_details(item,operand_item):
    if(item == 'MASKmskw'):
       operand_item.kind= 'mask'
    elif(item.startswith('XMM')):
        operand_item.width = 128
        operand_item.kind='reg'
    elif (item.startswith('YMM')):
        operand_item.width = 256
        operand_item.kind='reg'
    elif (item.startswith('ZMM')):
        operand_item.width = 512
        operand_item.kind='reg'
    elif (item.startswith('MEM')):
        operand_item.kind='mem'
    elif(item.startswith('IMM')):
        operand_item.kind='imm'
        operand_item.width=8


def get_eff_operands(ins):
    if(ins.ins_isa == 'sse') and (ins.nb_operands >=2):
        ins.kind1=ins.operands[0].kind
        ins.kind2=ins.operands[1].kind
        first_op=eff_operand(0,(ins.operands[0]).kind)
        second_op=eff_operand(1,ins.operands[1].kind)
        ins.eff_operands.append(first_op)
        ins.eff_operands.append(second_op)
    elif (ins.ins_isa == 'avx') and (ins.nb_operands >=3):
        ins.index1=1
        ins.index2=2
        ins.kind1=ins.operands[1].kind
        ins.kind2=ins.operands[2].kind
        first_op=eff_operand(1,(ins.operands[1]).kind)
        second_op=eff_operand(2,(ins.operands[2]).kind)
        ins.eff_operands.append(first_op)
        ins.eff_operands.append(second_op)


def isfma(mnemonic):
    if( not isfma_amd(mnemonic) ):
        if(mnemonic.startswith("VFMADD")) or (mnemonic.startswith("VFMADDSUB")) or (mnemonic.startswith("VFNMADD")) or (mnemonic.startswith("VFMSUB")) or (mnemonic.startswith("VFMSUBADD")) or (mnemonic.startswith("VFNMSUB")):
            return True
        else:
            return False
    else:
        return False


def isfma_amd(mnemonic):
    if(mnemonic.startswith("VFMADD") or mnemonic.startswith("VFNMADD") or mnemonic.startswith("VFMSUB") or mnemonic.startswith("VFNMSUB")):
        if(len(mnemonic) < 9):
            return True
        else:
            if(mnemonic.startswith("VFMADDSUB") or mnemonic.startswith("VFMSUBADD")):
                if(len(mnemonic) < 12):
                    return True
                else:
                    return False
            else:
                return False
    else:
        return False


def fill_fma_order_list(mnemonic):
    order_list=[]
    if(isfma(mnemonic)):
          order_list.append(int(mnemonic[-5]))
          order_list.append(int(mnemonic[-4]))
          order_list.append(int(mnemonic[-3]))
    return order_list


# parses the tokens, calls all the previous functions in order to form python objects       
def token_parser(pin_file_path,list_sse,list_avx,list_avx512, list_fma):
    pin_file=open(pin_file_path,'r')
    lines=pin_file.read()
    pin_file.close()  
    list_tokens=get_tokens_list(pin_file_path)
    for line in lines.split('\n'):
        if line.lstrip().startswith("XED_IFORM_")== True :
            token=line.lstrip().split('=')[0]
            if token in list_tokens:
                splits= token.split('_')[2:]
                values = [item.value for item in Op]
                op_code,op_type=get_opcode_optype(token)
                if(op_type in list_operations):
                    if (op_code!=None):
                        ins=instruction(token)
                        ins.fma_order=fill_fma_order_list(op_code)
                        ins.isfma_avx512=check_if_fma_and_avx512(token)
                        ins.ins_isa=get_isa(token)
                        ins.opcode, ins.optype=op_code,op_type
                        ins.ins_precision=get_precision(ins.opcode)
                        ins.simd_option=get_simd(ins.opcode)
                        operands_string_list=get_operand_string_list(token)
                        for item in operands_string_list:
                            operand_item=operand()
                            get_operand_details(item,operand_item)
                            ins.operands.append(operand_item)
                        ins.nb_operands=len(ins.operands)
                        if ins.simd_option == 'scalar':
                            ins.nb_elements=1
                        else:
                            ins.nb_elements=ins.operands[0].width//ins.ins_precision.nb_bits
                        get_eff_operands(ins)
                        if(ins.ins_isa == 'sse'):
                            list_sse.append(ins)
                        elif(ins.ins_isa == 'avx'):
                            list_avx.append(ins)
                        elif(ins.ins_isa == 'avx512'):
                            list_avx512.append(ins)
                        elif(ins.ins_isa == 'fma'):
                            list_fma.append(ins)
                        

# To run the code generator, needed arguments: path of the pin enum file, name of the template file, name of the output path
if __name__ == "__main__":
    pin_file_path=sys.argv[1] 
    template_file=sys.argv[2] 
    output=sys.argv[3]
    
    wrappers="wrappers"
    output_path = os.path.join(output,wrappers)
    path_exists=os.path.exists(output_path)
    if not path_exists:
        os.makedirs(output_path)
        
    output_file_sse=os.path.join(output_path,"sse.h")
    output_file_avx=os.path.join(output_path,"avx.h")
    output_file_avx512=os.path.join(output_path,"avx512.h")
    output_file_fma=os.path.join(output_path,"fma.h")
    
    
    instructions_list_sse=[]
    instructions_list_avx=[]
    instructions_list_avx512=[]
    instructions_list_fma=[]
    token_parser(pin_file_path,instructions_list_sse,instructions_list_avx,instructions_list_avx512, instructions_list_fma)
    total_size = len(instructions_list_sse) + len(instructions_list_avx) + len(instructions_list_avx512) + len(instructions_list_fma)


    env = Environment(loader=FileSystemLoader('templates'),autoescape=False, trim_blocks=True,lstrip_blocks=True)
    template = env.get_template(template_file)
   
    with open(output_file_sse, 'w+') as f:
        f.write(template.render(instructions=instructions_list_sse,architecture_name='sse'))
    with open(output_file_avx, 'w+') as f:
        f.write(template.render(instructions=instructions_list_avx,architecture_name='avx'))
    with open(output_file_avx512, 'w+') as f:
        f.write(template.render(instructions=instructions_list_avx512,architecture_name='avx512'))
    with open(output_file_fma, 'w+') as f:
        f.write(template.render(instructions=instructions_list_fma,architecture_name='fma'))


   

        
        



