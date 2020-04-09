# -*- coding: utf-8 -*-
import unittest
import subprocess
import os
from pathlib import Path

####################### path #######################

#workplace path for simplification, replace by your own path
workpath = str(Path(os.path.abspath(__file__)).parent.parent)

#executable path 
execpath = workpath + '/Test/sampleprog/x64/Debug/sampleprog.exe'

#path for pin executable
pinpath = workpath + '/Pin/Windows/pin.exe'

#path for pintool
toolpath = workpath + '/out/build/x64-Debug/Addcount/Addcount.dll'

#print(out.stdout.decode('utf-8').replace("\r\n","")) for not using intermediate log file

####################### unittest #######################
        
class PinToolTest(unittest.TestCase):
    """first basic test on PinTool, see path below"""
        
    def test_version(self):
        """Test -version for displaying the actual version"""
        #exec 
        out = subprocess.run([pinpath, '-t', toolpath, '-version', '1', '--', execpath], stdout=subprocess.PIPE, shell = True)
        out = (out.stdout.decode('utf-8')).splitlines()
        
        #getting result in ref
        with open(workpath + '/Test/versionref.txt') as ref:
            result = ref.read()
            result = result.splitlines()

        #testing output with reference
        self.assertEqual(out,result)
        #self.assertNotEqual(result,out)


if __name__ == '__main__':
    unittest.main()

