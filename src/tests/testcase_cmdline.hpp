#ifndef TESTS_TESTCASECMDLINE
#define TESTS_TESTCASECMDLINE

#include "util/unittest.hpp"

class TestCaseCmdLine : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseCmdLine)
    virtual void registerTestFunctions();
    
    //Tests
    void testCmdLineParser1();
    void testCmdLineParser2();
    void testCmdLineParser3();
    void testCmdLineMaker();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASECMDLINE
