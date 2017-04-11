#ifndef TESTS_TESTCASEJSON
#define TESTS_TESTCASEJSON

#include "util/unittest.hpp"

class TestCaseJson : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseJson)
    virtual void registerTestFunctions();
    
    //Tests
    void testJsonToString();
    void testStringToJson();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASEJSON
