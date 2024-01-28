#include "unity.h"
#include "unity_fixture.h"
#include "main.h"

#include "Reader.h"
#include "Analayzer.h"
#include "CircuralBuffer.h"
#define FILETOLOG "/home/dawid_zadlo/Desktop/Test/procstat.txt"
#define BUFFORMAXLEN 255
char outputstream[256];


static CPUstats* procStatInfo = NULL;
static Analyzer_S* analyzerStruct = NULL;
static circuralbuffer* bufferToPrint= NULL;

static unsigned char cores = 0;
pthread_mutex_t mutex,printer_mtx;
pthread_cond_t analyze,print;
unsigned char cpuCores;
atomic_bool done = 0;
_Atomic(int)  wdg[THREADSTOCALL];
circuralbuffer* bufforToPrint =NULL;


static unsigned char FakeReader_ReadCores(void)
{

    char buffor[BUFFORMAXLEN];
    char pattern[] = "cpu";
    unsigned char  res = 0;
    FILE* pFile = NULL;
    //home/dawid_zadlo/Desktop/Test/procstat.txt
    
    pFile = fopen(FILETOLOG,"r");
    if (pFile==NULL)
    {

        exit(EXIT_FAILURE);
    
    }
    else
    {   
        fgets(buffor,BUFFORMAXLEN,pFile); // skipping first line 
        /* Reading one by each line to the end of the file*/
        while(fgets(buffor,BUFFORMAXLEN,pFile))
        {
            /* Loooking for pattern "cpu" in buffor if it maches increment res*/
            if(strspn(buffor,pattern)==strlen(pattern))
            {
                res+=1;
            }
        }

    }
    fclose(pFile);
    pFile = NULL;
    if (res == 0)
    {

        exit(EXIT_FAILURE);
    }
    return res;
}


static void FakeReader_ReadCoresInfo(CPUstats object[static 1],const unsigned char howManyCores)
{
    assert(object!=NULL);
    char buffor[BUFFORMAXLEN];
    FILE* pFile = NULL;
    pFile = fopen("/home/dawid_zadlo/Desktop/Test/procstat.txt","r");
    if(howManyCores==0)
    {

        fclose(pFile);
        done=1;
        pthread_exit(0);
    }
    
    if (pFile==NULL)
    {

        fclose(pFile);
        done=1;
        pthread_exit(0);
    }

    else
    {   
        fgets(buffor,BUFFORMAXLEN,pFile); // skipping first line 
        
        /* Reading one by each line to the end of the file*/
        for(int i=0;i<howManyCores;i++)
        {
            fscanf(pFile,"%s %ld %ld %ld %ld %ld %ld %ld %ld",object->core,&object->user,&object->nice,&object->system,&object->idle,&object->iowait,&object->irq,&object->softirq,&object->steal);
            while(fgetc(pFile)!='\n'); // Has to be implemented because we dont read whole line so we have to skip last chars
            ++object;
        }
    
    }
    fclose(pFile);
    pFile = NULL;
}

TEST_GROUP(Reader_TestGR);
TEST_SETUP(Reader_TestGR)
{
};
TEST_TEAR_DOWN(Reader_TestGR)
{

};


TEST(Reader_TestGR,ReadHowManyCores)
{
    cores = FakeReader_ReadCores(); 
    TEST_ASSERT_EQUAL_CHAR(6,cores);
}

TEST(Reader_TestGR,InitCPUStatObject)
{
    procStatInfo = Read_InitReaderStruct(cores);
    TEST_ASSERT_NOT_NULL(procStatInfo);

}
TEST(Reader_TestGR,DeInitCPUStatObject)
{
    Reader_DeInitReaderStruct(&procStatInfo);
    TEST_ASSERT_NULL(procStatInfo);
}


TEST(Reader_TestGR,ReadFakeCPUStats)
{
    FakeReader_ReadCoresInfo(procStatInfo,cores);
    CPUstats procStatInfoArray[cores];
    for( int i= 0; i <cores;i++)
    {
        procStatInfoArray[i] = (*(procStatInfo+i));
    }
    TEST_ASSERT_EQUAL(194476,procStatInfoArray[0].user);
    TEST_ASSERT_EQUAL(166172,procStatInfoArray[cores-1].user);
}


TEST_GROUP_RUNNER(Reader_TestGR)
{
    RUN_TEST_CASE(Reader_TestGR,ReadHowManyCores);
    RUN_TEST_CASE(Reader_TestGR,InitCPUStatObject);
    RUN_TEST_CASE(Reader_TestGR,ReadFakeCPUStats);

    
}


TEST_GROUP(Analyzer_TestGR);
TEST_SETUP(Analyzer_TestGR)
{
};
TEST_TEAR_DOWN(Analyzer_TestGR)
{

};

TEST(Analyzer_TestGR,AnalyzerInitStruct)
{
    analyzerStruct = Analyzer_InitAnalyzerStruct(cores,procStatInfo);
    TEST_ASSERT_NOT_NULL(analyzerStruct);
}
TEST(Analyzer_TestGR,DeInitAnalyzerStruct)
{
    Analyzer_DeInitAnalyzerStruct(&analyzerStruct);
    TEST_ASSERT_NULL(analyzerStruct);
}

TEST(Analyzer_TestGR,AnalyzerCalculatePercentageUsage)
{
    float ret  =  CalculatePercentageUsage(procStatInfo,0);
    TEST_ASSERT_EQUAL_FLOAT(27.48762,ret);
}

TEST(Analyzer_TestGR,ANalyzerCalcualteUsage)
{
    CPUstats ret = CalculateUsage(procStatInfo,procStatInfo,0);
    TEST_ASSERT_EQUAL(0,ret.idle);
    TEST_ASSERT_EQUAL(0,ret.iowait);
    TEST_ASSERT_EQUAL(0,ret.irq);
    TEST_ASSERT_EQUAL(0,ret.nice);
    TEST_ASSERT_EQUAL(0,ret.softirq);
    TEST_ASSERT_EQUAL(0,ret.steal);
    TEST_ASSERT_EQUAL(0,ret.system);
    TEST_ASSERT_EQUAL(0,ret.user);
}

TEST_GROUP_RUNNER(Analyzer_TestGR)
{
    RUN_TEST_CASE(Analyzer_TestGR,AnalyzerInitStruct);
    RUN_TEST_CASE(Analyzer_TestGR,AnalyzerCalculatePercentageUsage);
    RUN_TEST_CASE(Analyzer_TestGR,ANalyzerCalcualteUsage);
    RUN_TEST_CASE(Analyzer_TestGR,DeInitAnalyzerStruct);
}




int main(void)
{
    UNITY_BEGIN();
    RUN_TEST_GROUP(Reader_TestGR);
    RUN_TEST_GROUP(Analyzer_TestGR);
    RUN_TEST_CASE(Reader_TestGR,DeInitCPUStatObject);
    UNITY_END();
}