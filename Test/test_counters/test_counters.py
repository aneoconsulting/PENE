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

    def compareOutputWithReference_result(self, runargs, referenceFileName):
        out = subprocess.run(runargs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell = True)
        output = out.stdout.decode('utf-8')
        splittedOutput = output.splitlines()
        print(output) 
        with open(self.workpath + '/' + referenceFileName) as ref:
            result = ref.read()
            result = result.splitlines()
        self.assertEqual(splittedOutput[18],result[0])

    def compareOutputWithReference_loop(self, runargs, referenceFileName):
        out = subprocess.run(runargs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell = True)
        output = out.stdout.decode('utf-8')
        splittedOutput = output.splitlines()
        print(output) 
        with open(self.workpath + '/' + referenceFileName) as ref:
            result = ref.read()
            result = result.splitlines()
        self.assertEqual(splittedOutput[16],result[0])
    
    def test_default(self):
        """Test -counter-mode for using the counters pin tool"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath], "test_counters_reference_active.txt")

    def test_loop(self):
        """Test only if the counters pin tool disrupts the number of operations performed by the programme"""
        self.compareOutputWithReference_loop([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "add", "double", "scalar", "97", "0", "0"], "test_counters_reference_loop.txt")

    def test_result(self):
        """Test only if the counters pin tool disrupts the validity of the programme"""
        self.compareOutputWithReference_result([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "add", "float", "scalar", "10", "100", "10"], "test_counters_reference_result.txt")

    def test_inactive(self):
        """Test that counter is not displayed with the flag is equal to 0"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '0', '--', self.execpath], "test_counters_reference_inactive.txt")

if __name__ == '__main__':
    unittest.main()

