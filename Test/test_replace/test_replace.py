import unittest
import subprocess
from pathlib import Path
import os
import re

        
class replace_tests(unittest.TestCase):
    execpath = "@REPLACE_TESTS_ECEXUTABLE@"
    pinpath = "${PIN_EXECUTABLE}"
    toolpath = "@REPLACE_TESTS_PINTOOL@"
    a = 3.5
    b = 7.2
    activeRegex = str(a)+r"\*"+str(b)+"="+str(a*b)
    activePattern = re.compile(activeRegex)
    inactiveRegex = str(a)+r"\*"+str(b)+"="+str(a+b)
    inactivePattern = re.compile(inactiveRegex)
        

    def checkOutputWithRegex(self, runargs, pattern, noppatern):
        out = subprocess.run(runargs, stdout=subprocess.PIPE, stderr = subprocess.STDOUT, shell = True)
        output = out.stdout.decode('utf-8')
        print(output)
        self.assertRegex(output, pattern)
        self.assertNotRegex(output, noppatern)
    
    def test_replace(self):
        """Test -replace for switching between add and mul"""
        self.checkOutputWithRegex([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '-fp-replace', '101', '--', self.execpath, str(self.a), str(self.b)], self.activePattern, self.inactivePattern)

    def test_replace_inactive(self):
        """Test -replace for switching between add and mul"""
        self.checkOutputWithRegex([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '-fp-replace', '0', '--', self.execpath, str(self.a), str(self.b)], self.inactivePattern, self.activePattern)

if __name__ == '__main__':
    unittest.main()

