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
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath], "test_counters_reference_default.txt")

    def test_loop(self):
        """Test only if the counters pin tool disrupts the number of operations performed by the programme"""
        self.compareOutputWithReference_loop([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "add", "double", "scalar", "97", "0", "0"], "test_counters_reference_loop.txt")

    def test_result(self):
        """Test only if the counters pin tool disrupts the validity of the programme"""
        self.compareOutputWithReference_result([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "add", "float", "scalar", "10", "100", "10"], "test_counters_reference_result.txt")

    def test_inactive(self):
        """Test that counter is not displayed with the flag is equal to 0"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '0', '--', self.execpath], "test_counters_reference_inactive.txt")

class count_tests(unittest.TestCase):
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

       
    def test_add_float_scalar(self):
        """Test the validity of couting add_float_scalar"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "add", "float", "scalar", "10", "0", "0"], "test_counters_reference_add_float_scalar.txt")
        
    def test_mul_float_scalar(self):
        """Test the validity of couting mul_float_scalar"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "mul", "float", "scalar", "10", "0", "0"], "test_counters_reference_mul_float_scalar.txt")
        
    def test_div_float_scalar(self):
        """Test the validity of couting div_float_scalar"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "div", "float", "scalar", "10", "0", "1"], "test_counters_reference_div_float_scalar.txt")
        
    def test_fma_float_scalar(self):
        """Test the validity of couting fma_float_scalar"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "fma", "float", "scalar", "10", "0", "0"], "test_counters_reference_fma_float_scalar.txt")

    def test_add_double_scalar(self):
        """Test the validity of couting add_double_scalar"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "add", "double", "scalar", "10", "0", "0"], "test_counters_reference_add_double_scalar.txt")

    def test_mul_double_scalar(self):
        """Test the validity of couting mul_double_scalar"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "mul", "double", "scalar", "10", "0", "0"], "test_counters_reference_mul_double_scalar.txt")
        
    def test_div_double_scalar(self):
        """Test the validity of couting div_double_scalar"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "div", "double", "scalar", "10", "0", "1"], "test_counters_reference_div_double_scalar.txt")
        
    def test_fma_double_scalar(self):
        """Test the validity of couting fma_double_scalar"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "fma", "double", "scalar", "10", "0", "0"], "test_counters_reference_fma_double_scalar.txt")
              
    def test_add_float_simd(self):
        """Test the validity of couting add_float_simd"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "add", "float", "simd", "10", "0", "0"], "test_counters_reference_add_float_simd.txt")
              
    def test_mul_float_simd(self):
        """Test the validity of couting mul_float_simd"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "mul", "float", "simd", "10", "0", "0"], "test_counters_reference_mul_float_simd.txt")
        
    def test_div_float_simd(self):
        """Test the validity of couting div_float_simd"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "div", "float", "simd", "10", "0", "1"], "test_counters_reference_div_float_simd.txt")
        
    def test_fma_float_simd(self):
        """Test the validity of couting fma_float_simd"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "fma", "float", "simd", "10", "0", "0"], "test_counters_reference_fma_float_simd.txt")
        
    def test_add_double_simd(self):
        """Test the validity of couting add_double_simd"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "add", "double", "simd", "10", "0", "0"], "test_counters_reference_add_double_simd.txt")

    def test_mul_double_simd(self):
        """Test the validity of couting mul_double_simd"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "mul", "double", "simd", "10", "0", "0"], "test_counters_reference_mul_double_simd.txt")
        
    def test_div_double_simd(self):
        """Test the validity of couting div_double_simd"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "div", "double", "simd", "10", "0", "1"], "test_counters_reference_div_double_simd.txt")
        
    def test_fma_double_simd(self):
        """Test the validity of couting fma_double_simd"""
        self.compareOutputWithReference([self.pinpath, '-t', self.toolpath, '-counter-mode', '1', '--', self.execpath, "fma", "double", "simd", "10", "0", "0"], "test_counters_reference_fma_double_simd.txt")



if __name__ == '__main__':
    unittest.main()

