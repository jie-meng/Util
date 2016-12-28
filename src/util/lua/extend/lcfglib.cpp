#include "lcfglib.hpp"
#include "util/luaextend.hpp"
#include "util/cfg.hpp"

namespace util
{

using namespace std;

const std::string kTextCfgHandle = "TextCfg*";

static int textcfgCreate(lua_State* plua_state)
{
    LuaObject<TextCfg>* p = luaNewEmptyObject<TextCfg>(plua_state, kTextCfgHandle);
    p->setData(new TextCfg(luaGetString(plua_state, 1, "")));

    return 1;
}

static int textcfgDestroy(lua_State* plua_state)
{
    return luaObjectDestroy<TextCfg>(plua_state, kTextCfgHandle);
}

static int textcfgGetCfgFile(lua_State* plua_state)
{
    TextCfg* ptextcfg = luaGetObjectData<TextCfg>(plua_state, kTextCfgHandle);
    luaPushString(plua_state, ptextcfg->getCfgFile());
    
    return 1;
}

static int textcfgLoad(lua_State* plua_state)
{
    TextCfg* ptextcfg = luaGetObjectData<TextCfg>(plua_state, kTextCfgHandle);
    ptextcfg->load(luaGetString(plua_state, 2));
    
    return 0;
}

static int textcfgSave(lua_State* plua_state)
{
    TextCfg* ptextcfg = luaGetObjectData<TextCfg>(plua_state, kTextCfgHandle);
    string save_file = luaGetString(plua_state, 2, "");
    if (save_file.empty())
        ptextcfg->save();
    else
        ptextcfg->save(save_file);
    
    return 0;
}

static int textcfgGetValue(lua_State* plua_state)
{
    TextCfg* ptextcfg = luaGetObjectData<TextCfg>(plua_state, kTextCfgHandle);
    luaPushString(plua_state, ptextcfg->getValue(luaGetString(plua_state, 2), luaGetString(plua_state, 3)));
    
    return 1;
}

static int textcfgSetValue(lua_State* plua_state)
{
    TextCfg* ptextcfg = luaGetObjectData<TextCfg>(plua_state, kTextCfgHandle);
    ptextcfg->setValue(luaGetString(plua_state, 2), luaGetString(plua_state, 3));
    
    return 0;
}

static int textcfgToString(lua_State* plua_state)
{
    return luaObjectToString<TextCfg>(plua_state, kTextCfgHandle);
}

static const u_luaL_Reg cfg_lib[] =
{
    {"createTextCfg", textcfgCreate},

    {0, 0}
};

static const u_luaL_Reg textcfg_obj_lib[] = {
    {"destroy", textcfgDestroy},
    {"getCfgFile", textcfgGetCfgFile},
    {"load", textcfgLoad},
    {"save", textcfgSave},
    {"getValue", textcfgGetValue},
    {"setValue", textcfgSetValue},
    {"__gc", textcfgDestroy},
    {"__tostring", textcfgToString},
    
    {0, 0}
};

int lualibCfgCreate(lua_State* plua_state) 
{
    luaCreateLib(plua_state, (u_luaL_Reg*)cfg_lib);
    luaCreateMeta(plua_state, kTextCfgHandle, (u_luaL_Reg*)textcfg_obj_lib);
    return 1;
}

}
