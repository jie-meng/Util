#ifndef UTIL_LUA_CSV_LIB_HPP
#define UTIL_LUA_CSV_LIB_HPP

#include <string>

struct lua_State;

namespace util
{

const std::string kLuaExtendLibCsv = "csv";

int lualibCsvCreate(lua_State* plua_state);

}

#endif // UTIL_LUA_CSV_LIB_HPP
