#ifndef UTIL_LUA_EXTEND_NET_LIB_HPP
#define UTIL_LUA_EXTEND_NET_LIB_HPP

#include <string>

struct lua_State;

namespace util
{

const std::string kLuaExtendLibNet = "net";

int lualibNetCreate(lua_State* plua_state);

}

#endif // UTIL_LUA_EXTEND_NET_LIB_HPP
