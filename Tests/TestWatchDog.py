import time
import os
"""
TEST CASE

Thread printer sleeps for 4000s
Test checks if watchdog will terminate program after 2s if thread will stuck 
"""
def main():
   
    currentDir = os.getcwd()
    
   

    os.chdir(currentDir)
    start = time.time()
    os.system("./CPU_Analyzer")
    end = time.time()
    result = round((end-start))
    # Result 
    if (result==3):
        
        print("Watchdog test passed\n")
        return 0

    else:
        
        print("Watchdog test failed\n")
        return -1

if __name__ =='__main__':
    main()