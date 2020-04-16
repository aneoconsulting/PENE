import unittest
import subprocess
from pathlib import Path
import os

        
class flag_tests(unittest.TestCase):
    workpath = "@FLAG_TESTS_BINARY_DIR@"
    execpath = "@FLAG_TESTS_ECEXUTABLE@"
    pinpath = "@PIN_EXECUTABLE@"
    toolpath = "@FLAG_TESTS_PINTOOL@"
        

    def compareOutputWithReference(self, runargs, referenceFileName):
        out = subprocess.run(runargs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell = True)
        output = out.stdout.decode('utf-8')
        splittedOutput = output.splitlines()
        print(output) 

        with open(self.workpath + '/' + referenceFileName) as ref:
            result = ref.read()
            result = result.splitlines()
 
        self.assertEqual(splittedOutput,result)
    
    def test_active(self):
        """Test -version for displaying the actual version"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-version', '1', '--', self.execpath], "test_version_reference_active.txt")

    def test_inactive(self):
        """Test that version is not displayed without the -version flag"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '0', '--', self.execpath], "test_version_reference_inactive.txt")

if __name__ == '__main__':
    unittest.main()
