#include "lregexlib.hpp"
#include "util/luaextend.hpp"
#include "util/regex.hpp"

namespace util
{

const std::string kRegexHandle = "Regex*";

static int create(lua_State* plua_state)
{
    LuaObject<Regex>* p = luaNewEmptyObject<Regex>(plua_state, kRegexHandle);
    p->setData(new Regex(luaGetString(plua_state, 1, ""), (Regex::RegexFlag)luaGetInteger(plua_state, 2, 0)));

    return 1;
}

static int destroy(lua_State* plua_state)
{
    return luaObjectDestroy<Regex>(plua_state, kRegexHandle);
}

static int compile(lua_State* plua_state)
{
    Regex* pregexer = luaGetObjectData<Regex>(plua_state, kRegexHandle);
    pregexer->compile(luaGetString(plua_state, 2));

    return 0;
}

static int match(lua_State* plua_state)
{
    Regex* pregexer = luaGetObjectData<Regex>(plua_state, kRegexHandle);
    luaPushBoolean(plua_state, pregexer->match(luaGetString(plua_state, 2)));

    return 1;
}

static int search(lua_State* plua_state)
{
    Regex* pregexer = luaGetObjectData<Regex>(plua_state, kRegexHandle);
    luaPushBoolean(plua_state, pregexer->search(luaGetString(plua_state, 2)));

    return 1;
}

static int getMatchedGroupCnt(lua_State* plua_state)
{
    Regex* pregexer = luaGetObjectData<Regex>(plua_state, kRegexHandle);
    luaPushInteger(plua_state, pregexer->getMatchedGroupCnt());

    return 1;
}

static int getMatchedGroupByIndex(lua_State* plua_state)
{
    Regex* pregexer = luaGetObjectData<Regex>(plua_state, kRegexHandle);
    luaPushString(plua_state, pregexer->getMatchedGroup(luaGetInteger(plua_state, 2)));

    return 1;
}

static int getMatchedGroupByName(lua_State* plua_state)
{
    Regex* pregexer = luaGetObjectData<Regex>(plua_state, kRegexHandle);
    luaPushString(plua_state, pregexer->getMatchedGroup(luaGetString(plua_state, 2)));

    return 1;
}

static int toString(lua_State* plua_state)
{
    return luaObjectToString<Regex>(plua_state, kRegexHandle);
}

static const u_luaL_Reg regex_lib[] =
{
    {"create", create},

    {0, 0}
};

static const u_luaL_Reg regex_obj_lib[] = {
    {"destroy", destroy},
    {"compile", compile},
    {"match", match},
    {"search", search},
    {"getMatchedGroupCnt", getMatchedGroupCnt},
    {"getMatchedGroupByIndex", getMatchedGroupByIndex},
    {"getMatchedGroupByName", getMatchedGroupByName},
    {"__gc", destroy},
    {"__tostring", toString},
    
    {0, 0}
};

int lualibRegexCreate(lua_State* plua_state) 
{
    luaCreateLib(plua_state, (u_luaL_Reg*)regex_lib);
    luaCreateMeta(plua_state, kRegexHandle, (u_luaL_Reg*)regex_obj_lib);
    return 1;
}

}
