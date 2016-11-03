#include "unittest_hoster.hpp"
#include "testcase_string.hpp"
#include "testcase_file.hpp"
#include "testcase_lexicalcast.hpp"
#include "testcase_csv.hpp"

using namespace std;
using namespace util;

UnitTestHoster::UnitTestHoster()
{
}

void UnitTestHoster::run()
{
    TestSuite suit;
    
    suit.addTestSuite(TestCaseString("TestCaseString"));
    suit.addTestSuite(TestCaseFile("TestCaseFile"))
    suit.addTestSuite(TestCaseLexicalCast("TestCaseLexicalCast"));
    suit.addTestSuite(TestCaseCsv("TestCaseCsv"));

    TestRunner runner;
    runner.run(suit);
}
