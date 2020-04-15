# -*- coding: utf-8 -*-
import unittest
import subprocess
from pathlib import Path
import os

        
class flag_tests(unittest.TestCase):
    workpath = "@COUNTERS_TESTS_BINARY_DIR@"
    execpath = "@COUNTERS_TESTS_ECEXUTABLE@"
    toolpath = "@COUNTERS_TESTS_PINTOOL@"
    pinpath = "@PIN_EXECUTABLE@"
        

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
        """Test -counter-mode for using the counters pin tool"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath], "test_counters_reference_active.txt")

    def test_inactive(self):
        """Test that counter is not displayed with the flag is equal to 0"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '0', '--', self.execpath], "test_counters_reference_inactive.txt")

if __name__ == '__main__':
    unittest.main()

