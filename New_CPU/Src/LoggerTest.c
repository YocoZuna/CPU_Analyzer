#include "unity.h"
#include "unity_fixture.h"
#include "main.h"
#include "Loger.h"
#define FAKEFILETOLOG "/home/dawid_zadlo/Desktop/Test/Src/FakeLogs.txt"


static char returnBuffor[MESSEGESIZE];

static char*  StubGetTime(void)
{
    
    return "Thu Jan 18 10:21:53 2024\n";
}

static void FakeLogToFile(const char dataToLog[])
{
    FILE* pFile = NULL;
    pFile = fopen(FAKEFILETOLOG,"a+");
 
    if (pFile==NULL)
    {
        exit(0);
    }
    else
    {   
        
        //time_t t;  
        //time(&t);
        fprintf(pFile,"When occured:%s %s\n",StubGetTime(),dataToLog);

    }
    fclose(pFile);
    pFile = NULL;
}

static void SpyLoggerGetFileContent(void)
{
    static int count =0;
    FILE* pFile = NULL;
    pFile = fopen(FAKEFILETOLOG,"r");
 
    if (pFile==NULL)
    {
        exit(0);
    }
    else
    {   
        ++count;
        for(int i=0;i<count;++i)
        {
            fgets(returnBuffor,MESSEGESIZE,pFile);
        }
    }
    fclose(pFile);
    pFile = NULL;
}

static void SpyLoggerPutToConsole(void)
{
 strncpy(returnBuffor,Circural_Deque(bufforToPrint),MESSEGESIZE);

}

#define CLEARFAKELOG    system("cd ~"); \
                        system("cd /home/dawid_zadlo/Desktop/Test/Src"); \
                        system("rm FakeLogs.txt"); 
atomic_bool done = 0;
_Atomic(int)  wdg[THREADSTOCALL];
circuralbuffer* bufforToPrint =NULL;
static char tempBuf[MESSEGESIZE];

TEST_GROUP(Logger_TestGR);
TEST_SETUP(Logger_TestGR)
{


    bufforToPrint = Circural_InitBuffer();
};
TEST_TEAR_DOWN(Logger_TestGR)
{    

    Circural_DeInitBuffer(&bufforToPrint);
};
TEST(Logger_TestGR,LogToFileTest)
{
    FakeLogToFile("Logger test\n");
    SpyLoggerGetFileContent();
    TEST_ASSERT_EQUAL_STRING("When occured:Thu Jan 18 10:21:53 2024\n",returnBuffor);
    SpyLoggerGetFileContent();
    TEST_ASSERT_EQUAL_STRING(" Logger test\n",returnBuffor);

};

TEST(Logger_TestGR,PutToConsoleTest)
{
    putToConsole("%s","This is test putToConsole");
    SpyLoggerPutToConsole();
    TEST_ASSERT_EQUAL_STRING("This is test putToConsole",returnBuffor);
};


TEST_GROUP_RUNNER(Logger_TestGR)
{

    RUN_TEST_CASE(Logger_TestGR,LogToFileTest);
    RUN_TEST_CASE(Logger_TestGR,PutToConsoleTest);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST_GROUP(Logger_TestGR);

    UNITY_END();
}