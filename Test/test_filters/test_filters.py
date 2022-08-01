import unittest
import subprocess
from pathlib import Path
import os
import re

        
class filters_tests(unittest.TestCase):
    execpath = "@EXECUTABLE@"
    pinpath = "${PIN_EXECUTABLE}"
    toolpath = "@PINTOOL@"


    def get_value(self, output, name):
        p = re.compile(r"^{}\([^:]*?= (?P<value>-?\d+\.?\d*?)".format(name), re.MULTILINE)
        self.assertRegex(output, p)
        m = p.search(output)
        return float(m.group("value"))

    def checkOutputWithRegex(self, runargs):
        out = subprocess.run(runargs, stdout=subprocess.PIPE, stderr = subprocess.STDOUT)
        print("the commandline is {}".format(subprocess.list2cmdline(out.args)))
        output = out.stdout.decode('utf-8')
        print(output)
        return output
    
    def test_excl_all(self):
        output = self.checkOutputWithRegex([self.pinpath, '-t', self.toolpath, '-fp-replace', '102', '-exclude', 'excl_all.lst', '--', self.execpath])
        self.assertEqual(5, self.get_value(output, "shared_func_f::compute1"))
        self.assertEqual(5, self.get_value(output, "shared_func_f::compute2"))
        self.assertEqual(5, self.get_value(output, "shared_func_f::"))
        self.assertEqual(5, self.get_value(output, "shared_func_f"))        
        self.assertEqual(5, self.get_value(output, "func_f::compute1"))
        self.assertEqual(5, self.get_value(output, "func_f::compute2"))
        self.assertEqual(5, self.get_value(output, "func_f"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute1"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute2"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::"))
        self.assertEqual(5, self.get_value(output, "shared_func_d"))        
        self.assertEqual(5, self.get_value(output, "func_d::compute1"))
        self.assertEqual(5, self.get_value(output, "func_d::compute2"))
        self.assertEqual(5, self.get_value(output, "func_d"))
    
    def test_excl_double(self):
        output = self.checkOutputWithRegex([self.pinpath, '-t', self.toolpath, '-fp-replace', '102', '-exclude', 'excl_double.lst', '--', self.execpath])
        self.assertEqual(-2, self.get_value(output, "shared_func_f::compute1"))
        self.assertEqual(-8, self.get_value(output, "shared_func_f::compute2"))
        self.assertEqual(8, self.get_value(output, "shared_func_f::"))
        self.assertEqual(8, self.get_value(output, "shared_func_f"))        
        self.assertEqual(-2, self.get_value(output, "func_f::compute1"))
        self.assertEqual(-8, self.get_value(output, "func_f::compute2"))
        self.assertEqual(8, self.get_value(output, "func_f"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute1"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute2"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::"))
        self.assertEqual(5, self.get_value(output, "shared_func_d"))        
        self.assertEqual(5, self.get_value(output, "func_d::compute1"))
        self.assertEqual(5, self.get_value(output, "func_d::compute2"))
        self.assertEqual(5, self.get_value(output, "func_d"))
        
    def test_excl_shared(self):
        output = self.checkOutputWithRegex([self.pinpath, '-t', self.toolpath, '-fp-replace', '102', '-exclude', 'excl_shared.lst', '--', self.execpath])
        self.assertEqual(5, self.get_value(output, "shared_func_f::compute1"))
        self.assertEqual(5, self.get_value(output, "shared_func_f::compute2"))
        self.assertEqual(5, self.get_value(output, "shared_func_f::"))
        self.assertEqual(5, self.get_value(output, "shared_func_f"))        
        self.assertEqual(-2, self.get_value(output, "func_f::compute1"))
        self.assertEqual(-8, self.get_value(output, "func_f::compute2"))
        self.assertEqual(8, self.get_value(output, "func_f"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute1"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute2"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::"))
        self.assertEqual(5, self.get_value(output, "shared_func_d"))        
        self.assertEqual(-2, self.get_value(output, "func_d::compute1"))
        self.assertEqual(-8, self.get_value(output, "func_d::compute2"))
        self.assertEqual(8, self.get_value(output, "func_d"))
        
    def test_excl_shared_all(self):
        output = self.checkOutputWithRegex([self.pinpath, '-t', self.toolpath, '-fp-replace', '102', '-exclude', 'excl_shared_all.lst', '--', self.execpath])
        self.assertEqual(5, self.get_value(output, "shared_func_f::compute1"))
        self.assertEqual(5, self.get_value(output, "shared_func_f::compute2"))
        self.assertEqual(5, self.get_value(output, "shared_func_f::"))
        self.assertEqual(5, self.get_value(output, "shared_func_f"))        
        self.assertEqual(-2, self.get_value(output, "func_f::compute1"))
        self.assertEqual(-8, self.get_value(output, "func_f::compute2"))
        self.assertEqual(8, self.get_value(output, "func_f"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute1"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute2"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::"))
        self.assertEqual(5, self.get_value(output, "shared_func_d"))        
        self.assertEqual(-2, self.get_value(output, "func_d::compute1"))
        self.assertEqual(-8, self.get_value(output, "func_d::compute2"))
        self.assertEqual(8, self.get_value(output, "func_d"))
        
    def test_excl_shared_double(self):
        output = self.checkOutputWithRegex([self.pinpath, '-t', self.toolpath, '-fp-replace', '102', '-exclude', 'excl_shared_double.lst', '--', self.execpath])
        self.assertEqual(-2, self.get_value(output, "shared_func_f::compute1"))
        self.assertEqual(-8, self.get_value(output, "shared_func_f::compute2"))
        self.assertEqual(8, self.get_value(output, "shared_func_f::"))
        self.assertEqual(8, self.get_value(output, "shared_func_f"))        
        self.assertEqual(-2, self.get_value(output, "func_f::compute1"))
        self.assertEqual(-8, self.get_value(output, "func_f::compute2"))
        self.assertEqual(8, self.get_value(output, "func_f"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute1"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute2"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::"))
        self.assertEqual(5, self.get_value(output, "shared_func_d"))        
        self.assertEqual(-2, self.get_value(output, "func_d::compute1"))
        self.assertEqual(-8, self.get_value(output, "func_d::compute2"))
        self.assertEqual(8, self.get_value(output, "func_d"))

    @unittest.expectedFailure
    def test_excl_tree_func_f(self):
        output = self.checkOutputWithRegex([self.pinpath, '-t', self.toolpath, '-fp-replace', '102', '-exclude-tree', 'excl_func_f.lst', '--', self.execpath])
        self.assertEqual(-2, self.get_value(output, "shared_func_f::compute1"))
        self.assertEqual(-8, self.get_value(output, "shared_func_f::compute2"))
        self.assertEqual(8, self.get_value(output, "shared_func_f::"))
        self.assertEqual(8, self.get_value(output, "shared_func_f"))        
        self.assertEqual(-2, self.get_value(output, "func_f::compute1"))
        self.assertEqual(-8, self.get_value(output, "func_f::compute2"))
        self.assertEqual(8, self.get_value(output, "func_f"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute1"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::compute2"))
        self.assertEqual(5, self.get_value(output, "shared_func_d::"))
        self.assertEqual(5, self.get_value(output, "shared_func_d"))        
        self.assertEqual(5, self.get_value(output, "func_d::compute1"))
        self.assertEqual(5, self.get_value(output, "func_d::compute2"))
        self.assertEqual(5, self.get_value(output, "func_d"))

        

if __name__ == '__main__':
    unittest.main()

