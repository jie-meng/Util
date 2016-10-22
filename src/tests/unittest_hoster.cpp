#include "unittest_hoster.hpp"
#include "testcase_string.hpp"

using namespace std;
using namespace util;

UnitTestHoster::UnitTestHoster()
{
}

void UnitTestHoster::run()
{
    TestSuite suit;
    
    suit.addTestSuite(TestCaseString("TestCaseString"));

    TestRunner runner;
    runner.run(suit);
}
