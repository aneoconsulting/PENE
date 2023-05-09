import subprocess
from pathlib import Path
import os
import re

        
class TestBackendVerrou():
    execpath = "@EXECUTABLE@"
    pinpath = "${PIN_EXECUTABLE}"
    toolpath = "@FLAG_TESTS_PINTOOL@"

    """Expected value for the test"""
    out_ex_downward = r"4599075939470750515"
    out_ex_downward_pattern=re.compile(out_ex_downward)
  
        

    def checkOutput(self,runargs, pattern):
        out = subprocess.run(runargs, stdout=subprocess.PIPE, stderr = subprocess.STDOUT)
        print("the commandline is {}".format(subprocess.list2cmdline(out.args)))
        output = out.stdout.decode('utf-8')
        if re.search(pattern,output):
            return 0
        else:
            return 1
        
    
    
    def test_ex_downward(self):
       """Test -rounding mode downward on a chosen value"""
       self.checkOutput([self.pinpath, '-t', self.toolpath, '-fp-replace', '3', '-rounding-mode', '3', '--', self.execpath], self.out_ex_downward_pattern)
