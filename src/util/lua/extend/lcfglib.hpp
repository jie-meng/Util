#ifndef UTIL_LUA_EXTEND_CFG_LIB_HPP
#define UTIL_LUA_EXTEND_CFG_LIB_HPP

#include <string>

struct lua_State;

namespace util
{

const std::string kLuaExtendLibCfg = "cfg";

int lualibCfgCreate(lua_State* plua_state);

}

#endif // UTIL_LUA_EXTEND_CFG_LIB_HPP
