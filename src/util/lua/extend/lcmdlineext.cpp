#include "lcmdlineext.hpp"
#include "util/luaextend.hpp"
#include "util/cmdline.hpp"

namespace util
{

using namespace std;

const std::string kCmdLineParserHandle = "CmdLineParser*";
const std::string kCmdLineMakerHandle = "CmdLineMaker*";

// CmdLineParser    
static int cmdLineParserCreate(lua_State* plua_state)
{
    LuaObject<CmdLineParser>* p = luaNewEmptyObject<CmdLineParser>(plua_state, kCmdLineParserHandle);
    p->setData(
        new CmdLineParser(
            luaToBoolean(plua_state, 1, false),
            luaToString(plua_state, 2, " -"),
            luaToString(plua_state, 3, " "),
            luaToString(plua_state, 4, "\""),
            luaToString(plua_state, 5, "\""),
            luaToBoolean(plua_state, 6, true)));

    return 1;
}

static int cmdLineParserDestroy(lua_State* plua_state)
{
    return luaObjectDestroy<CmdLineParser>(plua_state, kCmdLineParserHandle);
}

static int cmdLineParserToString(lua_State* plua_state)
{
    return luaObjectToString<CmdLineParser>(plua_state, kCmdLineParserHandle);    
}

static int cmdLineParserIsCaseSensitive(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    luaPushBoolean(plua_state, pcmdline_parser->isCaseSensitive());
    
    return 1;
}

static int cmdLineParserGetCmd(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    luaPushString(plua_state, pcmdline_parser->getCmd());
    
    return 1;
}

static int cmdLineParserGetCmdLine(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    luaPushString(plua_state, pcmdline_parser->getCmdLine());
    
    return 1;
}

static int cmdLineParserGetRet(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    luaPushString(plua_state, pcmdline_parser->getRet());
    
    return 1;
}

static int cmdLineParserGetContent(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    luaPushString(plua_state, pcmdline_parser->getContent());
    
    return 1;
}

static int cmdLineParserParse(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    luaPushBoolean(plua_state, pcmdline_parser->parse(luaToString(plua_state, 2)));
    
    return 1;
}

static int cmdLineParserHasKey(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    luaPushBoolean(plua_state, pcmdline_parser->hasKey(luaToString(plua_state, 2)));
    
    return 1;
}

static int cmdLineParserRemoveKey(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    pcmdline_parser->removeKey(luaToString(plua_state, 2));
    
    return 0;
}

static int cmdLineParserGetValue(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    luaPushString(plua_state, pcmdline_parser->getValue(luaToString(plua_state, 2), luaToString(plua_state, 3, "")));
    
    return 1;
}

// CmdLineMaker
static int cmdLineMakerCreate(lua_State* plua_state)
{
    LuaObject<CmdLineMaker>* p = luaNewEmptyObject<CmdLineMaker>(plua_state, kCmdLineMakerHandle);
    p->setData(
        new CmdLineMaker(
            luaToBoolean(plua_state, 1, false),
            luaToString(plua_state, 2, " -"),
            luaToString(plua_state, 3, " ")));

    return 1;
}

static int cmdLineMakerDestroy(lua_State* plua_state)
{
    return luaObjectDestroy<CmdLineMaker>(plua_state, kCmdLineMakerHandle);
}

static int cmdLineMakerToString(lua_State* plua_state)
{
    return luaObjectToString<CmdLineMaker>(plua_state, kCmdLineMakerHandle);    
}

static int cmdLineMakerIsCaseSensitive(lua_State* plua_state)
{
    CmdLineMaker* pcmdline_maker = luaGetObjectData<CmdLineMaker>(plua_state, kCmdLineMakerHandle);
    luaPushBoolean(plua_state, pcmdline_maker->isCaseSensitive());
    
    return 1;
}

static int cmdLineMakerGetCmd(lua_State* plua_state)
{
    CmdLineMaker* pcmdline_maker = luaGetObjectData<CmdLineMaker>(plua_state, kCmdLineMakerHandle);
    luaPushString(plua_state, pcmdline_maker->getCmd());
    
    return 1;
}

static int cmdLineMakerSetCmd(lua_State* plua_state)
{
    CmdLineMaker* pcmdline_maker = luaGetObjectData<CmdLineMaker>(plua_state, kCmdLineMakerHandle);
    pcmdline_maker->setCmd(luaToString(plua_state, 2));
    
    return 0;
}

static int cmdLineMakerGetCmdLine(lua_State* plua_state)
{
    CmdLineMaker* pcmdline_maker = luaGetObjectData<CmdLineMaker>(plua_state, kCmdLineMakerHandle);
    luaPushString(plua_state, pcmdline_maker->getCmdLine());
    
    return 1;
}

static int cmdLineMakerRemoveKey(lua_State* plua_state)
{
    CmdLineMaker* pcmdline_maker = luaGetObjectData<CmdLineMaker>(plua_state, kCmdLineMakerHandle);
    pcmdline_maker->removeKey(luaToString(plua_state, 2));
    
    return 0;
}

static int cmdLineMakerSort(lua_State* plua_state)
{
    CmdLineMaker* pcmdline_maker = luaGetObjectData<CmdLineMaker>(plua_state, kCmdLineMakerHandle);
    
    std::vector<string> args;
    for (int i=2; i<=luaGetTop(plua_state); ++i)
        args.push_back(luaToString(plua_state, i));
    
    pcmdline_maker->sort(args.begin(), args.end());
    
    return 0;
}

static int cmdLineMakerAddKeyValue(lua_State* plua_state)
{
    CmdLineMaker* pcmdline_maker = luaGetObjectData<CmdLineMaker>(plua_state, kCmdLineMakerHandle);
    pcmdline_maker->addKeyValue(luaToString(plua_state, 2), luaToString(plua_state, 3));
    
    return 0;
}

static const LuaReg cmdline_lib[] =
{
    {"newCmdLineParser", cmdLineParserCreate},
    {"newCmdLineMaker", cmdLineMakerCreate},

    {0, 0}
};

static const LuaReg cmdlineparser_obj_lib[] = {
    {"delete", cmdLineParserDestroy},
    {"isCaseSensitive", cmdLineParserIsCaseSensitive},
    {"getCmd", cmdLineParserGetCmd},
    {"getCmdLine", cmdLineParserGetCmdLine},
    {"getRet", cmdLineParserGetRet},
    {"getContent", cmdLineParserGetContent},
    {"parse", cmdLineParserParse},
    {"hasKey", cmdLineParserHasKey},
    {"removeKey", cmdLineParserRemoveKey},
    {"getValue", cmdLineParserGetValue},
    {"__gc", cmdLineParserDestroy},
    {"__tostring", cmdLineParserToString},
    
    {0, 0}
};

static const LuaReg cmdlinemaker_obj_lib[] = {
    {"delete", cmdLineMakerDestroy},
    {"isCaseSensitive", cmdLineMakerIsCaseSensitive},
    {"getCmd", cmdLineMakerGetCmd},
    {"setCmd", cmdLineMakerSetCmd},
    {"getCmdLine", cmdLineMakerGetCmdLine},
    {"removeKey", cmdLineMakerRemoveKey},
    {"addKeyValue", cmdLineMakerAddKeyValue},
    {"sort", cmdLineMakerSort},
    {"__gc", cmdLineMakerDestroy},
    {"__tostring", cmdLineMakerToString},
    
    {0, 0}
};

void extendCmdLine(lua_State* plua_state) 
{
    LuaRegCombUtilLib::getInstance().addRegArray((LuaReg*)cmdline_lib);
    luaCreateMeta(plua_state, kCmdLineParserHandle, (LuaReg*)cmdlineparser_obj_lib);
    luaCreateMeta(plua_state, kCmdLineMakerHandle, (LuaReg*)cmdlinemaker_obj_lib);
}
    
} //util
