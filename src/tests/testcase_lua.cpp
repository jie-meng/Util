#include "testcase_lua.hpp"
#include "util/luax.hpp"

using namespace std;
using namespace util;

void TestCaseLua::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseLua, testLuaStateGetGlobal)
    REGISTER_TEST_FUNCTION(TestCaseLua, testLuaStateSetGlobal)
}

void TestCaseLua::setUp()
{
}

void TestCaseLua::tearDown()
{    
}

void TestCaseLua::testLuaStateGetGlobal()
{
    LuaState ls;
    ls.parseLine("a = 3");
    assertEquals<int>(0, luaGetTop(ls.getState()), ASSERT_POSITION);
    luaGetGlobal(ls.getState(), "a");
    assertEquals<int>(1, luaGetTop(ls.getState()), ASSERT_POSITION);
    assertEquals<int>(3, luaGetInteger(ls.getState(), 1), ASSERT_POSITION);
    assertEquals<int>(1, luaGetTop(ls.getState()), ASSERT_POSITION);
    luaPop(ls.getState(), -1);
    assertEquals<int>(0, luaGetTop(ls.getState()), ASSERT_POSITION);
}

void TestCaseLua::testLuaStateSetGlobal()
{
    LuaState ls;
    assertEquals<int>(0, luaGetTop(ls.getState()), ASSERT_POSITION);
    luaGetGlobal(ls.getState(), "a");
    assertEquals<int>(1, luaGetTop(ls.getState()), ASSERT_POSITION);
    assertEquals<int>(0, luaGetInteger(ls.getState(), 1, 0), ASSERT_POSITION);
    luaPop(ls.getState(), -1);
    assertEquals<int>(0, luaGetTop(ls.getState()), ASSERT_POSITION);
    
    luaPushString(ls.getState(), "test");
    assertEquals<int>(1, luaGetTop(ls.getState()), ASSERT_POSITION);
    luaSetGlobal(ls.getState(), "a");
    assertEquals<int>(0, luaGetTop(ls.getState()), ASSERT_POSITION);
    luaGetGlobal(ls.getState(), "a");
    assertEquals<int>(1, luaGetTop(ls.getState()), ASSERT_POSITION);
    assertEquals<string>("test", luaGetString(ls.getState(), 1, ""), ASSERT_POSITION);
    luaPop(ls.getState(), -1);
    assertEquals<int>(0, luaGetTop(ls.getState()), ASSERT_POSITION);
}