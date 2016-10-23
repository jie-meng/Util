#ifndef TESTS_TESTCASESTRING
#define TESTS_TESTCASESTRING

#include "util/unittest.hpp"

class TestCaseString : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseString)
    virtual void registerTestFunctions();
    
    //Tests
    void testIsSpace();
    void testStrFormat();
    void testStrToLower();
    void testStrToUpper();
    void testStrTrimLeftEx();
    void testStrTrimRightEx();
    void testStrTrimEx();
    void testStrTrim();
    void testStrReplace();
    void testStrReplaceAll();
    void testStrStartWith();
    void testStrEndWith();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASESTRING
