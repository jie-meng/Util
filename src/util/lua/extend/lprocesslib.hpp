#ifndef UTIL_LUA_PROCESS_LIB_HPP
#define UTIL_LUA_PROCESS_LIB_HPP

struct lua_State;

namespace util
{

void extendProcess(lua_State* plua_state);

}
#endif // UTIL_LUA_PROCESS_LIB_HPP
