#pragma once
#include <pin.H>
extern "C"
{
# include <xed-decoded-inst-api.h>
}
#include<iostream>


namespace pene 
{
  namespace replace
  {
    namespace vfc_wrappers
    {
      namespace devectorized_sse
      {
        
        template<typename T, int N>
        void arithmetic_wrapper(T* const a, T* const b, T* c, interflop_backend_interface_t * backends, void** contexts, int loaded_backends, op_t op)
        {
          void (*interflop_call[MAX_BACKENDS])(float,float,float*,void*);
          switch (op)
          {
          case ADD_1:
          case ADD_4:
            for (int i = 0; i < loaded_backends; i++)
            {

              interflop_call[i] = backends[i].interflop_add_float;
            }
            break;
          case SUB_1:
          case SUB_4:
            for (int i = 0; i < loaded_backends; i++)
            {

              interflop_call[i] = backends[i].interflop_sub_float;
            }
            break;
          case MUL_1:
          case MUL_4:
            for (int i = 0; i < loaded_backends; i++)
            {

              interflop_call[i] = backends[i].interflop_mul_float;
            }
            break;
          case DIV_1:
          case DIV_4:
            for (int i = 0; i < loaded_backends; i++)
            {

              interflop_call[i] = backends[i].interflop_div_float;
            }
            break;
          default:
            break;
          }
          
          for (int i = 0; i < loaded_backends; i++)
          {
            for(int s = 0; s < N; s++)
            {
              interflop_call[i](*(a+s), *(b+s), c+s, contexts[i]);
            }
          }
        }

        void instrument(int loaded_backends, interflop_backend_interface_t* backends, void** contexts, INS ins)
        {
          auto xed = INS_XedDec(ins); // TODO check that pointer does not need to be freed
          auto iform = xed_decoded_inst_get_iform_enum(xed);

          switch (iform)
          { 
            case xed_iform_enum_t::XED_IFORM_ADDPS_XMMps_MEMps:
            {                
              {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float, 4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, ADD_4,
              IARG_END
              );
              }
            INS_Delete(ins);
            break;
            }

            
            case xed_iform_enum_t::XED_IFORM_ADDPS_XMMps_XMMps:
            {
              {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float, 4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, ADD_4,
              IARG_END
              );
              }
            INS_Delete(ins);
            break;
            }
            case xed_iform_enum_t::XED_IFORM_ADDSS_XMMss_MEMss:
            {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, ADD_1,
              IARG_END
              );
            }
            INS_Delete(ins);
            break;
            case xed_iform_enum_t::XED_IFORM_ADDSS_XMMss_XMMss:
            {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, ADD_1,
              IARG_END
              );
            }
            INS_Delete(ins);
            break;
            case xed_iform_enum_t::XED_IFORM_SUBPS_XMMps_MEMps:
            {                
              {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, SUB_4,
              IARG_END
              );
              }
            INS_Delete(ins);
            break;
            }

            
            case xed_iform_enum_t::XED_IFORM_SUBPS_XMMps_XMMps:
            {
              {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, SUB_4,
              IARG_END
              );
              }
            INS_Delete(ins);
            break;
            }
            case xed_iform_enum_t::XED_IFORM_SUBSS_XMMss_MEMss:
            {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, SUB_1,
              IARG_END
              );
            }
            INS_Delete(ins);
            break;
            case xed_iform_enum_t::XED_IFORM_SUBSS_XMMss_XMMss:
            {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, SUB_1,
              IARG_END
              );
            }
            INS_Delete(ins);
            break;
            case xed_iform_enum_t::XED_IFORM_MULPS_XMMps_MEMps:
            {                
              {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, MUL_4,
              IARG_END
              );
              }
            INS_Delete(ins);
            break;
            }

            
            case xed_iform_enum_t::XED_IFORM_MULPS_XMMps_XMMps:
            {
              {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, MUL_4,
              IARG_END
              );
              }
            INS_Delete(ins);
            break;
            }
            case xed_iform_enum_t::XED_IFORM_MULSS_XMMss_MEMss:
            {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, MUL_1,
              IARG_END
              );
            }
            INS_Delete(ins);
            break;
            case xed_iform_enum_t::XED_IFORM_MULSS_XMMss_XMMss:
            {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, MUL_1,
              IARG_END
              );
            }
            INS_Delete(ins);
            break;
            case xed_iform_enum_t::XED_IFORM_DIVPS_XMMps_MEMps:
            {                
              {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, DIV_4,
              IARG_END
              );
              }
            INS_Delete(ins);
            break;
            }

            
            case xed_iform_enum_t::XED_IFORM_DIVPS_XMMps_XMMps:
            {
              {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, DIV_4,
              IARG_END
              );
              }
            INS_Delete(ins);
            break;
            }
            case xed_iform_enum_t::XED_IFORM_DIVSS_XMMss_MEMss:
            {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, DIV_1,
              IARG_END
              );
            }
            INS_Delete(ins);
            break;
            case xed_iform_enum_t::XED_IFORM_DIVSS_XMMss_XMMss:
            {
              INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)arithmetic_wrapper<float,1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, INS_OperandReg(ins,0),
              IARG_PTR, backends,
              IARG_PTR, contexts,
              IARG_UINT32, loaded_backends,
              IARG_UINT32, DIV_1,
              IARG_END
              );
            }
            INS_Delete(ins);
            break;
            default: break;
            

                  
          }
        }


      }
    }
  }
}
