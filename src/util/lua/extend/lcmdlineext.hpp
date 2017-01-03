#ifndef UTIL_LUA_CMDLINE_EXT_HPP
#define UTIL_LUA_CMDLINE_EXT_HPP

struct lua_State;

namespace util
{

void extendCmdLine(lua_State* plua_state);

}

#endif // UTIL_LUA_CMDLINE_EXT_HPP
