#ifndef UTIL_LUAEXTEND_HPP
#define UTIL_LUAEXTEND_HPP

#include "luax.hpp"

namespace util
{

struct u_luaL_Reg 
{
    const char *name;
    LuaCFunc func;
};

template <typename T>
class LuaObject 
{
public:
    //LuaObject does not need constructor & destructor because it would be created by luaNewUserData in lua_State
    inline T* getData() const { return pdata_; }
    inline void setData(T* pdata) { pdata_ = pdata; }
    inline void destroy() { safeDelete(pdata_); }
private:
  T* pdata_; 
};

void luaCreateMeta(lua_State * plua_state, const std::string& handleName, u_luaL_Reg* lr);

template <typename T>
LuaObject<T>* luaNewEmptyObject(lua_State* plua_state, const std::string& handleName)
{
    LuaObject<T>* p = (LuaObject<T>*)luaNewUserData(plua_state, sizeof(LuaObject<T>));
    p->setData(0);
    luaSetMetaTable(plua_state, handleName);
    return p;
}

template <typename T>
LuaObject<T>* luaGetObject(lua_State* plua_state,  const std::string& handleName)
{
    return (LuaObject<T>*)luaCheckUData(plua_state, 1, handleName);
}

template <typename T>
int luaObjectDestroy (lua_State *plua_state, const std::string& handleName)
{
    luaGetObject<T>(plua_state, handleName)->destroy();
    return 0;
}

template <typename T>
T* luaGetObjectData(lua_State* plua_state, const std::string& handleName) 
{
    LuaObject<T>* p = luaGetObject<T>(plua_state, handleName);
    if (p->getData() == 0)
    {
        luaError(plua_state, "Attempt to use a destoryed object");
        return 0;
    }
    
    return p->getData();
}

template <typename T>
int luaObjectToString(lua_State* plua_state, const std::string& handleName) 
{
    LuaObject<T>* p = luaGetObject<T>(plua_state, handleName);
    std::string typeName = strTrim(strReplaceAll(handleName, "*", ""));
    if (p->getData() == 0)
        luaPushString(plua_state, strFormat("%s (destroyed)", typeName.c_str()));
    else
        luaPushString(plua_state, strFormat("%s (%u)", typeName.c_str(), p->getData()));
    
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
