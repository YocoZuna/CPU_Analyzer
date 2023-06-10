cd Tests
Test1=$(python3 TestWatchDog.py)
Test2=$(python3 CountCPUTest.py)
cd ../build
echo "$Test1"
echo "$Test2"
sleep 2
./CPU_Analyzer

