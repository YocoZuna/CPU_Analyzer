#include "unity.h"
#include "unity_fixture.h"
#include  "main.h"

circuralbuffer* bufforToPrint;
static char toBigMessege[MESSEGESIZE+1];
static char messege[MESSEGESIZE];
static char tempBuf[MESSEGESIZE];
static char data;
static const char nothingToRead[] = "Nothing to read";
static char* InserData()
{
    int i = 0;
    static char temp[MESSEGESIZE];
    for(i; i <MESSEGESIZE-1;++i)
    {
        temp[i] = 'c';

    }
    temp[i] = '\0';
    return temp;
}


static inline void NothingToRead(void)
{
    strncpy(tempBuf,Circural_Deque(bufforToPrint),MESSEGESIZE);
    TEST_ASSERT_EQUAL_STRING(nothingToRead,tempBuf);
}

TEST_GROUP(Circural_TestGR);
TEST_SETUP(Circural_TestGR)
{
    data = 0;
    memset(tempBuf,'$',(sizeof(char)*MESSEGESIZE));
    strncpy(toBigMessege,messege,MESSEGESIZE);
    toBigMessege[MESSEGESIZE-1] = 'c';
    toBigMessege[MESSEGESIZE] = 'c';
};
TEST_TEAR_DOWN(Circural_TestGR)
{
};

TEST(Circural_TestGR,InitBuffer)
{
    bufforToPrint = Circural_InitBuffer();
    NothingToRead();
    TEST_ASSERT_NOT_NULL(bufforToPrint);
}
TEST(Circural_TestGR,DeInitBuffer)
{
Circural_DeInitBuffer(&bufforToPrint);
    TEST_ASSERT_NULL(bufforToPrint);
}
TEST(Circural_TestGR,InsertData)
{
    
    strcpy(messege,InserData());
    data = Circural_Enque(bufforToPrint,messege);
    TEST_ASSERT_EQUAL(0,data);


}

TEST(Circural_TestGR,Deque)
{

    strncpy(tempBuf,Circural_Deque(bufforToPrint),MESSEGESIZE);
    TEST_ASSERT_EQUAL_STRING(messege,tempBuf);

}  
TEST(Circural_TestGR,ToLongMessege)
{

    data = Circural_Enque(bufforToPrint,toBigMessege);
    TEST_ASSERT_EQUAL(-2,data);
    
}
TEST(Circural_TestGR,BufforLoop)
{
    bufforToPrint->head = 9;
    bufforToPrint->tail = 9;
    NothingToRead();
    
    strcpy(messege,"Buffor did the loop");
    data = Circural_Enque(bufforToPrint,messege);
    TEST_ASSERT_EQUAL(0,bufforToPrint->head);

    strncpy(tempBuf,Circural_Deque(bufforToPrint),MESSEGESIZE);

    TEST_ASSERT_EQUAL(0,bufforToPrint->tail);
    TEST_ASSERT_EQUAL_STRING(messege,tempBuf);
}



TEST_GROUP_RUNNER(Circural_TestGR)
{
    RUN_TEST_CASE(Circural_TestGR,InitBuffer);
    RUN_TEST_CASE(Circural_TestGR,InsertData);
    RUN_TEST_CASE(Circural_TestGR,Deque);
    RUN_TEST_CASE(Circural_TestGR,ToLongMessege);
    RUN_TEST_CASE(Circural_TestGR,BufforLoop);
    RUN_TEST_CASE(Circural_TestGR,DeInitBuffer);

}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST_GROUP(Circural_TestGR);

    UNITY_END();
}