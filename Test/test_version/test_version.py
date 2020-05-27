import unittest
import subprocess
from pathlib import Path
import os
import re

        
class flag_tests(unittest.TestCase):
    execpath = "@FLAG_TESTS_ECEXUTABLE@"
    pinpath = "${PIN_EXECUTABLE}"
    toolpath = "@FLAG_TESTS_PINTOOL@"

    activeRegex = r"This execution is launched with PENE version \d{1,2}.\d{1,2}[\r\n]{1,2}Execution will stop now.[\r\n]$\Z"
    activePattern = re.compile(activeRegex)
    inactiveRegex = r"[\r\n]{1,2}This program is only used for test purposes.[\r\n]{1,2}"
    inactivePattern = re.compile(inactiveRegex)
        

    def checkOutputWithRegex(self, runargs, pattern, noppatern):
        out = subprocess.run(runargs, stdout=subprocess.PIPE, stderr = subprocess.STDOUT, shell = True)
        print("the commandline is {}".format(subprocess.list2cmdline(out.args)))
        output = out.stdout.decode('utf-8')
        print(output)
        self.assertRegex(output, pattern)
        self.assertNotRegex(output, noppatern)
    
    def test_active(self):
        """Test -version for displaying the actual version"""
        self.checkOutputWithRegex([self.pinpath, '-t', self.toolpath, '-version', '1', '--', self.execpath], self.activePattern, self.inactivePattern)

    def test_inactive(self):
        """Test that version is not displayed without the -version flag"""
        self.checkOutputWithRegex([self.pinpath, '-t', self.toolpath, '--', self.execpath], self.inactivePattern, self.activePattern)

if __name__ == '__main__':
    unittest.main()

