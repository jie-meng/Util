#ifndef TESTS_TESTCASELEXICALCAST
#define TESTS_TESTCASELEXICALCAST

#include "util/unittest.hpp"

class TestCaseLexicalCast : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseLexicalCast)
    virtual void registerTestFunctions();
    
    //Tests
    void testLexicalCast();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASELEXICALCAST
