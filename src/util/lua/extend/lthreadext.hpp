#ifndef UTIL_LUA_THREAD_EXT_HPP
#define UTIL_LUA_THREAD_EXT_HPP

#include <string>

struct lua_State;

namespace util
{

void extendThread(lua_State* plua_state);

}

#endif // UTIL_LUA_THREAD_EXT_HPP
