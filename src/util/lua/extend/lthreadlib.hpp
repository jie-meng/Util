#ifndef UTIL_LUA_THREAD_LIB_HPP
#define UTIL_LUA_THREAD_LIB_HPP

#include <string>

struct lua_State;

namespace util
{

const std::string kLuaExtendLibThread = "thread";

int lualibThreadCreate(lua_State* plua_state);

}

#endif // UTIL_LUA_THREAD_LIB_HPP
