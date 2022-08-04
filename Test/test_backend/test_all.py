import subprocess
from pathlib import Path
import os
import re

        
class TestBackend():
    execpath1 = "@EXECUTABLE1@"
    execpath2 = "@EXECUTABLE2@"
    execpath3 = "@EXECUTABLE3@"
    pinpath = "${PIN_EXECUTABLE}"
    toolpath = "@FLAG_TESTS_PINTOOL@"


    out_ex1 = r"0x1.000002p+0"
    out_ex2=  r"0x1.000004p+2[\r\n]{ 1,2}0x1.800004p+1[\r\n]{ 1,2}0x1.000004p+1[\r\n]{ 1,2}0x1.000004p+0"
    out_ex1_pattern=re.compile(out_ex1)
    out_ex2_pattern=re.compile(out_ex2)
    out_ex3=r"0x1.000006p+5 , 0x1.e00006p+3 , 0x1.400006p+5 , 0x1.000006p+6 , 0x1.000006p+3 , 0x1.c00006p+3 , 0x1.e00006p+5 , 0x1.000006p+0[\r\n]{ 1,2}"
    out_ex3_pattern=re.compile(out_ex3)
    
  
        

    def checkOutput(self,runargs, pattern):
        out = subprocess.run(runargs, stdout=subprocess.PIPE, stderr = subprocess.STDOUT)
        print("the commandline is {}".format(subprocess.list2cmdline(out.args)))
        output = out.stdout.decode('utf-8')
        print(output)
        if re.search(pattern,output):
            return 0
        else:
            return 1
        
    
    
    def test_ex1(self):
       """Test -version for displaying the actual version"""
       self.checkOutput([self.pinpath, '-t', self.toolpath, '-fp-replace' , '4' , '--', self.execpath1],self.out_ex1_pattern)

    def test_ex2(self):
        """Test -version for displaying the actual version"""
        self.checkOutput([self.pinpath, '-t', self.toolpath, '-fp-replace' , '4' , '--', self.execpath2],self.out_ex2_pattern)

    def test_ex3(self):
        """Test -version for displaying the actual version"""
        self.checkOutput([self.pinpath, '-t', self.toolpath, '-fp-replace' , '4' , '--', self.execpath3],self.out_ex3_pattern)

   


    
        

