#ifndef UTIL_LUAEXTEND_HPP
#define UTIL_LUAEXTEND_HPP

#include "luax.hpp"

namespace util
{

typedef struct u_luaL_Reg {
    const char *name;
    LuaCFunc func;
} u_luaL_Reg;

//LuaExtender
class LuaExtender
{
public:
    LuaExtender() {}
    ~LuaExtender() {}
    void addLib(const std::string& name, LuaCFunc lib_create_func);
    void openLibs(lua_State* plua_state);
    void clearLibs() { libs_.clear(); }
private:
    typedef std::pair<std::string, LuaCFunc> LuaLib;
    std::vector<LuaLib> libs_;
private:
    DISALLOW_COPY_AND_ASSIGN(LuaExtender)
};

void luaCreateLib(lua_State* plua_state, u_luaL_Reg* lr);
void openUtilExtendLibs(lua_State* plua_state);
void addUtilExtendLib(const std::string& name, LuaCFunc lib_create_func);

//LuaExtendCmdLine
class LuaExtendCmdLine : public LuaCmdLine
{
public:
    LuaExtendCmdLine(const std::string& prompt = ">>>",
               const std::string& quit_cmd = "quit",
               bool is_case_sensitive = false);
    virtual ~LuaExtendCmdLine();
    virtual void clear();
private:
    DISALLOW_COPY_AND_ASSIGN(LuaExtendCmdLine)
};

void luaExecutor(int argc, char* argv[]);

void luaExtendError(lua_State* plua_state, const std::string& lib, const std::string& func, const std::string& msg);

void luaExtendAssert(lua_State* plua_state, const std::string& lib, const std::string& func, bool assertion, const std::string& msg);

} // namespace util

#endif // UTIL_LUAEXTEND_HPP
