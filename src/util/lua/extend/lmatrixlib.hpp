#ifndef UTIL_LUA_MATRIX_LIB_HPP
#define UTIL_LUA_MATRIX_LIB_HPP

#include <string>

struct lua_State;

namespace util
{

const std::string kLuaExtendLibMatrix = "matrix";

int lualibMatrixCreate(lua_State* plua_state);

}

#endif // UTIL_LUA_MATRIX_LIB_HPP
