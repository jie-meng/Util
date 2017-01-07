#ifndef TESTS_TESTCASEREGEX
#define TESTS_TESTCASEREGEX

#include "util/unittest.hpp"

class TestCaseRegex : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseRegex)
    virtual void registerTestFunctions();
    
    //Tests
    void testRegex();
    void testRegexIgnoreCase();
    void testRegexRightToLeft();
    void testRegexSingleLine();
    void testRegexMultiLine();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASEREGEX
