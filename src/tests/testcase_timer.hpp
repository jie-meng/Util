#ifndef TESTS_TESTCASETIMER
#define TESTS_TESTCASETIMER

#include "util/unittest.hpp"

class TestCaseTimer : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseTimer)
    virtual void registerTestFunctions();
    
    //Tests
    void testTimer();
    void testFuncTimer();
protected:
    virtual void setUp();
    virtual void tearDown();
private:
    void bindFunc();
    int count_;
};

#endif //TESTS_TESTCASETIMER
