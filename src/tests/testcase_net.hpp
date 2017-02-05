#ifndef TESTS_TESTCASENET
#define TESTS_TESTCASENET

#include "util/unittest.hpp"

class TestCaseNet : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseNet)
    virtual void registerTestFunctions();
    
    //Tests
    void testIpArrayToStrFail();
    void testIpArrayToStrSuccess();
    void testIpStrToArrayFail();
    void testIpStrToArraySuccess();
    void testNtohsAndHtons();
    void testNtohlAndHtonl();
    void testNtohllAndHtonll();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASENET
