import subprocess
import sys
import os

out = subprocess.run(sys.argv[1:], stdout=subprocess.PIPE, stderr = subprocess.STDOUT,text=True)
print(out.stdout)
sys.exit(out.returncode)
