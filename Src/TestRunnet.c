#include "unity.h"
#include "unity_fixture.h"
#include "main.h"
volatile sig_atomic_t done = 0;
TEST_GROUP(CPU_Analyzer);
TEST_SETUP(CPU_Analyzer)
{
    ;
}
TEST_TEAR_DOWN(CPU_Analyzer)
{
;
}

TEST(CPU_Analyzer,GetNumberOfCores)
{
    int howMany = Reader_ReadHowManyCPUs();
    TEST_ASSERT_EQUAL_INT(7,howMany);
}

TEST_GROUP_RUNNER(CPU_Analyzer)
{
    RUN_TEST_CASE(CPU_Analyzer,GetNumberOfCores);
}
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST_GROUP(CPU_Analyzer);
    UNITY_END();
}