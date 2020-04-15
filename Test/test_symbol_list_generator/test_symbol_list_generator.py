import re
import unittest
import subprocess

class symbols_list_generation_tests(unittest.TestCase):
    execpath = "@FLAG_TESTS_ECEXUTABLE@"
    pinpath = "@PIN_EXECUTABLE@"
    toolpath = "@FLAG_TESTS_PINTOOL@"


    elements = [["executable", ["main", "func2_2"]],
                ["shared2_funs", ["func2_1", "func2_2", "func2_3", "func1"]],
                ["shared_funs", ["func1", "func2", "func3"]]]

    def test_exec_and_shared(self):
        """Checks that the list generated contains the symbols included from executable"""
        runargs=[self.pinpath, '-t', self.toolpath, '-gen-sym-list', 'list.txt', '--', self.execpath]
        out = subprocess.run(runargs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell = True)
        
        regexes = [[re.compile(".+" + element[0] + ".+"), [re.compile(sym) for sym in element[1]]] for element in self.elements]
        
        with open('list.txt') as ref:
            list = ref.read()
            lines = list.splitlines()
            for line in lines:
                tmp = line.split('\t')
                object = tmp[0]
                symbol = tmp[1]
                obj_idx = None
                sym_idx = None
                for obj_index, mod_syms in enumerate(regexes):
                    m = mod_syms[0].match(object)
                    if m!=None:
                        for sym_index, sym in enumerate(mod_syms[1]):
                            m2 = sym.match(symbol)
                            if m2!=None :
                                print("Found " + line)
                                sym_idx = sym_index
                                obj_idx = obj_index
                                break
                        else:
                            continue
                        break
                if sym_idx!=None:
                    syms = regexes[obj_idx][1]
                    del syms[sym_idx]
                    if len(syms)==0:
                        del regexes[obj_idx]
                        if len(regexes) == 0:
                            return
        print(regexes)
        for elems in regexes:
            for sym in elems[1]:
                print("FAILED TO FIND: " + elems[0].pattern + "\t" + sym.pattern)
        self.fail()

        print(out.stdout.decode('utf-8'))

if __name__ == '__main__':
    unittest.main()






