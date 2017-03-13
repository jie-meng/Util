#include "testcase_lua.hpp"
#include "util/luax.hpp"

using namespace std;
using namespace util;

void TestCaseLua::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseLua, testLuaStateGetGlobal)
    REGISTER_TEST_FUNCTION(TestCaseLua, testLuaStateSetGlobal)
    REGISTER_TEST_FUNCTION(TestCaseLua, testLuaStateGetGlobalNew)
    REGISTER_TEST_FUNCTION(TestCaseLua, testLuaStateSetGlobalNew)
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
    assertEquals<int>(3, luaToInteger(ls.getState(), 1), ASSERT_POSITION);
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
    assertEquals<int>(0, luaToInteger(ls.getState(), 1, 0), ASSERT_POSITION);
    luaPop(ls.getState(), -1);
    assertEquals<int>(0, luaGetTop(ls.getState()), ASSERT_POSITION);
    
    luaPushString(ls.getState(), "test");
    assertEquals<int>(1, luaGetTop(ls.getState()), ASSERT_POSITION);
    luaSetGlobal(ls.getState(), "a");
    assertEquals<int>(0, luaGetTop(ls.getState()), ASSERT_POSITION);
    luaGetGlobal(ls.getState(), "a");
    assertEquals<int>(1, luaGetTop(ls.getState()), ASSERT_POSITION);
    assertEquals<string>("test", luaToString(ls.getState(), 1, ""), ASSERT_POSITION);
    luaPop(ls.getState(), -1);
    assertEquals<int>(0, luaGetTop(ls.getState()), ASSERT_POSITION);
}

void TestCaseLua::testLuaStateGetGlobalNew()
{
    LuaState ls;
    ls.parseLine("a = 3");
    assertEquals<int>(0, ls.getTop(), ASSERT_POSITION);
    ls.getGlobal("a");
    assertEquals<int>(1, ls.getTop(), ASSERT_POSITION);
    assertEquals<int>(3, ls.toInteger(1), ASSERT_POSITION);
    assertEquals<int>(1, ls.getTop(), ASSERT_POSITION);
    ls.pop(-1);
    assertEquals<int>(0, ls.getTop(), ASSERT_POSITION);
}

void TestCaseLua::testLuaStateSetGlobalNew()
{
    LuaState ls;
    assertEquals<int>(0, ls.getTop(), ASSERT_POSITION);
    ls.getGlobal("a");
    assertEquals<int>(1, ls.getTop(), ASSERT_POSITION);
    assertEquals<int>(0, ls.toInteger(1, 0), ASSERT_POSITION);
    ls.pop(-1);
    assertEquals<int>(0, ls.getTop(), ASSERT_POSITION);
    
    ls.pushString("test");
    assertEquals<int>(1, ls.getTop(), ASSERT_POSITION);
    ls.setGlobal("a");
    assertEquals<int>(0, ls.getTop(), ASSERT_POSITION);
    ls.getGlobal("a");
    assertEquals<int>(1, ls.getTop(), ASSERT_POSITION);
    assertEquals<string>("test", ls.toString(1, ""), ASSERT_POSITION);
    ls.pop(-1);
    assertEquals<int>(0, ls.getTop(), ASSERT_POSITION);
}
