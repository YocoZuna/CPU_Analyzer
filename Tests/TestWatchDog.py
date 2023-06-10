import time
import os
"""
TEST CASE

Thread printer sleeps for 4000s
Test checks if watchdog will terminate program after 2s if thread will stuck 
"""
start = time.time()

currentDir = os.getcwd()

TestDir=(currentDir+"/Tests")

os.chdir(TestDir)
os.system("./CPU_Analyzer")
end = time.time()
result = round((end-start))

if (result==2):
    print("Watchdog test passed\n")
else:
    print(result)
    print("Watchdog test failed\n")