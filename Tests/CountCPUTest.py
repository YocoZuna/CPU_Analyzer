import os 

def main():
    cpu = os.cpu_count()
    cpu = cpu*256

    TestedFunction = os.system("~/./TestCpu")
    if (TestedFunction==cpu):
        print("Read CPU test passed\n")
    else:
        print("Read CPU test failed\n")
    
if __name__ =='__main__':
    main()