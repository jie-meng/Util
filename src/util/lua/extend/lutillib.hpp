#ifndef UTIL_LUA_UTIL_LIB_HPP
#define UTIL_LUA_UTIL_LIB_HPP

struct lua_State;

namespace util
{

int lualibUtilCreate(lua_State* plua_state);

}

#endif // UTIL_LUA_UTIL_LIB_HPP