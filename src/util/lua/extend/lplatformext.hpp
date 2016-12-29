#ifndef UTIL_LUA_PLATFORM_EXT_HPP
#define UTIL_LUA_PLATFORM_EXT_HPP

struct lua_State;

namespace util
{

void extendPlatform(lua_State* plua_state);

}

#endif // UTIL_LUA_PLATFORM_EXT_HPP
