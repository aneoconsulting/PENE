# -*- coding: utf-8 -*-
import unittest
import subprocess

#C:\Users\fjia\Downloads\pin-3.11-97998-g7ecce2dac-msvc-windows\source\tools\MyPinTool


########################### path ###########################


#workplace path for simplification, replace by your own path
workpath = 'C:/Users/fjia/Downloads/pin-3.11-97998-g7ecce2dac-msvc-windows/source/tools/'

#pin executable path stock in path
execpath = 'C:/Users/fjia/Downloads/pin-3.11-97998-g7ecce2dac-msvc-windows/'
path = execpath + 'pin'

#tool extention path
toolpath = workpath + 'MyPinTool/Debug/MyPinTool.dll'

#target executable path
target = workpath + 'x64/Debug/sampleprog.exe'


#print(out.stdout.decode('utf-8').replace("\r\n","")) for case if not using intermediate log file



########################### unittest ###########################


        
class TestMyPinTool(unittest.TestCase):
    """unittest on MyPinTool, see path below"""
    
    def test_version(self):
        """Test method add(a,b)"""
        #exec 
        out = subprocess.run([path, '-t', toolpath, '--version', '--', target], stdout=subprocess.PIPE, shell = True)
        out = (out.stdout.decode('utf-8')).splitlines()
        
        #getting result in ref
        with open(workpath + 'version_log') as log:
            result = log.read()
            result = result.splitlines()

        #testing output with reference
        self.assertEqual(out,result)
        #self.assertNotEqual('v.0.2',result)


if __name__ == '__main__':
    unittest.main()

