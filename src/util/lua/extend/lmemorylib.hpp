#ifndef UTIL_LUA_EXTEND_MEMROY_LIB_HPP
#define UTIL_LUA_EXTEND_MEMROY_LIB_HPP

#include <string>

struct lua_State;

namespace util
{

const std::string kLuaExtendLibMemory = "memory";

int lualibMemoryCreate(lua_State* plua_state);

}

#endif // UTIL_LUA_EXTEND_MEMROY_LIB_HPP
