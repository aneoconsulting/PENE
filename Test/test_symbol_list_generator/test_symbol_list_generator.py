import re
import unittest
import subprocess

class symbols_list_generation_tests(unittest.TestCase):
    execpath = "@FLAG_TESTS_EXECUTABLE@"
    pinpath = "${PIN_EXECUTABLE}"
    toolpath = "@FLAG_TESTS_PINTOOL@"


    
    loaded = [["executable", ["main"]],
                ["shared2_funs", ["func2_1", "func2_2", "func2_3"]],
                ["shared_funs", ["func1", "func2", "func3"]]]

    executed = [["executable", ["main"]],
                ["shared2_funs", ["func2_1", "func2_2"]],
                ["shared_funs", ["func2"]]]


    def generate_and_validate_list(self, mode, symbols):
        runargs=[self.pinpath, '-t', self.toolpath, '-gen-sym-list', 'list'+mode+'.txt', '-gen-sym-mode', mode, '--', self.execpath]
        out = subprocess.run(runargs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        print("the commandline is {}".format(subprocess.list2cmdline(out.args)))
        print(out.stdout.decode('utf-8'))

        regexes = r"\A" + "".join([r"(?=.*?^\S*?" + elem[0] + r"\S*?\s+" + sym +r"$)" for elem in symbols for sym in elem[1]])
    
        with open('list'+mode+'.txt') as ref:
            input = ref.read()
            p = re.compile(regexes, re.MULTILINE | re.DOTALL)
            self.assertRegex(input, p)
        
                
    def test_loaded_symbols(self):
        """Checks that the list generated contains the symbols included from executable"""
        self.generate_and_validate_list('0', self.loaded)
            
    def test_exec_traces(self):
        """Checks that the list generated contains the symbols from executed traces from executable"""
        self.generate_and_validate_list('1', self.executed)

            
    def test_exec_bbls(self):
        """Checks that the list generated contains the symbols from executed BBLs from executable"""
        self.generate_and_validate_list('2', self.executed)




if __name__ == '__main__':
    unittest.main()






