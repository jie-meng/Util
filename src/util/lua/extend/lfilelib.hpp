#ifndef UTIL_LUA_FILE_LIB_HPP
#define UTIL_LUA_FILE_LIB_HPP

#include <string>

struct lua_State;

namespace util
{

const std::string kLuaExtendLibFile = "file";

int lualibFileCreate(lua_State* plua_state);

}

#endif // UTIL_LUA_FILE_LIB_HPP
