#ifndef TESTS_TESTCASEITERATOR
#define TESTS_TESTCASEITERATOR

#include "util/unittest.hpp"

class TestCaseIterator : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseIterator)
    virtual void registerTestFunctions();
    
    //Tests
    void testIterator();
    void testMapIterator();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASEITERATOR
