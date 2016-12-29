#ifndef UTIL_LUA_EXTEND_MEMROY_EXT_HPP
#define UTIL_LUA_EXTEND_MEMROY_EXT_HPP

struct lua_State;

namespace util
{

void extendMemory(lua_State* plua_state);

}

#endif // UTIL_LUA_EXTEND_MEMROY_EXT_HPP
