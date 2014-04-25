#ifndef UTIL_LUA_EXTEND_REGEX_LIB_HPP
#define UTIL_LUA_EXTEND_REGEX_LIB_HPP

#include <string>

struct lua_State;

namespace util
{

const std::string kLuaExtendLibRegex = "regex";

int lualibRegexCreate(lua_State* plua_state);

}

#endif // UTIL_LUA_EXTEND_REGEX_LIB_HPP
