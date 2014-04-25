#ifndef UTIL_LUA_PROCESS_LIB_HPP
#define UTIL_LUA_PROCESS_LIB_HPP

#include <string>

struct lua_State;

namespace util
{

const std::string kLuaExtendLibProcess = "process";

int lualibProcessCreate(lua_State* plua_state);

}
#endif // UTIL_LUA_PROCESS_LIB_HPP
