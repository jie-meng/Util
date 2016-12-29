#include "lstringext.hpp"
#include "util/luaextend.hpp"
#include "util/base.hpp"

namespace util
{

static int strTrimLeft(lua_State* plua_state)
{
    luaPushString(plua_state, util::strTrimLeft(luaGetString(plua_state, 1, "")));
    return 1;
}

static int strTrimRight(lua_State* plua_state)
{
    luaPushString(plua_state, util::strTrimRight(luaGetString(plua_state, 1, "")));
    return 1;
}

static int strTrim(lua_State* plua_state)
{
    luaPushString(plua_state, util::strTrim(luaGetString(plua_state, 1, "")));
    return 1;
}

static int strTrimLeftEx(lua_State* plua_state)
{
    luaPushString(plua_state, util::strTrimLeftEx(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, kWhiteSpace)));
    return 1;
}

static int strTrimRightEx(lua_State* plua_state)
{
    luaPushString(plua_state, util::strTrimRightEx(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, kWhiteSpace)));
    return 1;
}

static int strTrimEx(lua_State* plua_state)
{
    luaPushString(plua_state, util::strTrimEx(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, kWhiteSpace)));
    return 1;
}

static int strReplace(lua_State* plua_state)
{
    luaPushString(plua_state,
            util::strReplace(
                luaGetString(plua_state, 1, ""),
                luaGetString(plua_state, 2, ""),
                luaGetString(plua_state, 3, "")));

    return 1;
}

static int strRelaceAll(lua_State* plua_state)
{
    luaPushString(plua_state,
            util::strReplaceAll(
                luaGetString(plua_state, 1, ""),
                luaGetString(plua_state, 2, ""),
                luaGetString(plua_state, 3, "")));

    return 1;
}

static int strStartWith(lua_State* plua_state)
{
    luaPushBoolean(plua_state, util::strStartWith(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, ""), luaGetBoolean(plua_state, 3, true)));
    return 1;
}

static int strEndWith(lua_State* plua_state)
{
    luaPushBoolean(plua_state, util::strEndWith(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, ""), luaGetBoolean(plua_state, 3, true)));
    return 1;
}

static int strLeft(lua_State* plua_state)
{
    luaPushString(plua_state, util::strLeft(luaGetString(plua_state, 1, ""), luaGetInteger(plua_state, 2, 0)));
    return 1;
}

static int strRight(lua_State* plua_state)
{
    luaPushString(plua_state, util::strRight(luaGetString(plua_state, 1, ""), luaGetInteger(plua_state, 2, 0)));
    return 1;
}

static int strMid(lua_State* plua_state)
{
    //in lua, index start from 1, so mid index adjust to (idx-1)
    luaPushString(plua_state, util::strMid(luaGetString(plua_state, 1, ""), luaGetInteger(plua_state, 2, 0)-1, luaGetInteger(plua_state, 3, 0)));
    return 1;
}

static int strAreEqual(lua_State* plua_state)
{
    luaPushBoolean(plua_state, util::strAreEqual(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, ""), luaGetBoolean(plua_state, 3, true)));
    return 1;
}

static int strContains(lua_State* plua_state)
{
    luaPushBoolean(plua_state, util::strContains(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, ""), luaGetBoolean(plua_state, 3, true)));
    return 1;
}

static int strCompare(lua_State* plua_state)
{
    luaPushInteger(plua_state, util::strCompare(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, ""), luaGetBoolean(plua_state, 3, true)));
    return 1;
}

static int strJoin(lua_State* plua_state)
{
    std::vector< std::pair<any, any> > vec = luaGetTable(plua_state, 1);
    if (!vec.empty())
    {
        std::vector<std::string> str_vec;
        for (size_t i=0; i<vec.size(); ++i)
            str_vec.push_back(vec[i].second.toString());
        luaPushString(plua_state, util::strJoin(str_vec, luaGetString(plua_state, 2, "")));
    }
    else
    {
        luaPushString(plua_state, "");
    }

    return 1;
}

static int strSplit(lua_State* plua_state)
{
    std::vector<std::string> vec;
    util::strSplit(luaGetString(plua_state, 1, ""),
             luaGetString(plua_state, 2, " "),
             vec,
             luaGetInteger(plua_state, 3, 0));

    luaPushTable(plua_state, vec);

    return 1;
}

static int strSplitEx(lua_State* plua_state)
{
    std::vector<std::string> vec;
    util::strSplitEx(luaGetString(plua_state, 1, ""),
             luaGetString(plua_state, 2, " "),
             luaGetString(plua_state, 3, "\""),
             luaGetString(plua_state, 4, "\""),
             vec,
             luaGetInteger(plua_state, 5, 0));

    luaPushTable(plua_state, vec);

    return 1;
}

static const LuaReg string_lib[] =
{
    {"strTrim", strTrim},
    {"strTrimLeft", strTrimLeft},
    {"strTrimRight", strTrimRight},
    {"strTrimEx", strTrimEx},
    {"strTrimLeftEx", strTrimLeftEx},
    {"strTrimRightEx", strTrimRightEx},
    {"strReplace", strReplace},
    {"strRelaceAll", strRelaceAll},
    {"strLeft", strLeft},
    {"strRight", strRight},
    {"strMid", strMid},
    {"strAreEqual", strAreEqual},
    {"strContains", strContains},
    {"strCompare", strCompare},
    {"strStartWith", strStartWith},
    {"strEndWith", strEndWith},
    {"strJoin", strJoin},
    {"strSplit", strSplit},
    {"strSplitEx", strSplitEx},

    {0, 0}
};

void extendString(lua_State* plua_state) 
{
    LuaRegCombUtilLib::getInstance().addRegArray((LuaReg*)string_lib);
}

}
