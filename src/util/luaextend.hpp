#ifndef UTIL_LUAEXTEND_HPP
#define UTIL_LUAEXTEND_HPP

#include "luax.hpp"

namespace util
{

struct u_luaL_Reg {
    const char *name;
    LuaCFunc func;
};

struct LuaObject {
  void* pdata; 
  LuaCFunc destroy;
};

void luaCreateMeta(lua_State * plua_state, const std::string& handleName, u_luaL_Reg* lr);
LuaObject* luaNewEmptyObject(lua_State* plua_state, const std::string& handleName);
LuaObject* luaGetObject(lua_State* plua_state,  const std::string& handleName);
int luaObjectGc (lua_State *plua_state, const std::string& handleName);
int luaFileresult(lua_State* plua_state, bool stat, const std::string& fname = "");

template <typename T>
T* luaGetObjectHandle(lua_State* plua_state, const std::string& handleName) 
{
    LuaObject* p = luaGetObject(plua_state, handleName);
    if (p->destroy == NULL)
        luaError(plua_state, "Attempt to use a destoryed object");
    
    luaAssert(plua_state, p->pdata, "LuaObject data is NULL");
    return (T*)(p->pdata);
}

template <typename T>
int luaObjectToString (lua_State* plua_state, const std::string& handleName) 
{
    LuaObject* p = luaGetObject(plua_state, handleName);
    std::string typeName = strTrim(strReplaceAll(handleName, "*", ""));
    if (p->destroy == NULL)
        luaPushString(plua_state, strFormat("%s (destroyed)", typeName.c_str()));
    else
        luaPushString(plua_state, strFormat("%s (%u)", typeName.c_str(), p->pdata));
    return 1;
}

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
