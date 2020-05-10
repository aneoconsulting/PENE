import re
import unittest
import subprocess

class symbols_list_generation_tests(unittest.TestCase):
    execpath = "@FLAG_TESTS_ECEXUTABLE@"
    pinpath = "${PIN_EXECUTABLE}"
    toolpath = "@FLAG_TESTS_PINTOOL@"


    elements = [["executable", ["main", "func2_2"]],
                ["shared2_funs", ["func2_1", "func2_2", "func2_3", "func1"]],
                ["shared_funs", ["func1", "func2", "func3"]]]

        

    def test_exec_and_shared(self):
        """Checks that the list generated contains the symbols included from executable"""
        runargs=[self.pinpath, '-t', self.toolpath, '-gen-sym-list', 'list.txt', '--', self.execpath]
        out = subprocess.run(runargs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell = True)
        print(out.stdout.decode('utf-8'))

        regexes = r"\A" + "".join([r"(?=.*?^\S*?" + elem[0] + r"\S*?\s+" + sym +r"$)" for elem in self.elements for sym in elem[1]])
    
        with open('list.txt') as ref:
            input = ref.read()
            p = re.compile(regexes, re.MULTILINE | re.DOTALL)
            self.assertRegex(input, p)



if __name__ == '__main__':
    unittest.main()






