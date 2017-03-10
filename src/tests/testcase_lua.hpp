#ifndef TESTS_TESTCASELUA
#define TESTS_TESTCASELUA

#include "util/unittest.hpp"

class TestCaseLua : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseLua)
    virtual void registerTestFunctions();
    
    //Tests
    void testLuaStateGetGlobal();
    void testLuaStateSetGlobal();
    void testLuaStateGetGlobalNew();
    void testLuaStateSetGlobalNew();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASELUA
