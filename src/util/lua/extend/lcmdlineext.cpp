#include "lcmdlineext.hpp"
#include "util/luaextend.hpp"
#include "util/cmdline.hpp"

namespace util
{

using namespace std;

//const std::string kCmdLineHandle = "CmdLine*";
const std::string kCmdLineParserHandle = "CmdLineParser*";
const std::string kCmdLineMakerHandle = "CmdLineMaker*";
    
static int cmdLineParserCreate(lua_State* plua_state)
{
    LuaObject<CmdLineParser>* p = luaNewEmptyObject<CmdLineParser>(plua_state, kCmdLineParserHandle);
    p->setData(
        new CmdLineParser(
            luaGetBoolean(plua_state, 1, false),
            luaGetString(plua_state, 2, " -"),
            luaGetString(plua_state, 3, " "),
            luaGetString(plua_state, 4, "\""),
            luaGetString(plua_state, 5, "\""),
            luaGetBoolean(plua_state, 6, true)));

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
    luaPushBoolean(plua_state, pcmdline_parser->parse(luaGetString(plua_state, 2)));
    
    return 1;
}

static int cmdLineParserHasKey(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    luaPushBoolean(plua_state, pcmdline_parser->hasKey(luaGetString(plua_state, 2)));
    
    return 1;
}

static int cmdLineParserRemoveKey(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    pcmdline_parser->removeKey(luaGetString(plua_state, 2));
    
    return 0;
}

static int cmdLineParserGetValue(lua_State* plua_state)
{
    CmdLineParser* pcmdline_parser = luaGetObjectData<CmdLineParser>(plua_state, kCmdLineParserHandle);
    luaPushString(plua_state, pcmdline_parser->getValue(luaGetString(plua_state, 2), luaGetString(plua_state, 3, "")));
    
    return 1;
}

static const LuaReg cmdline_lib[] =
{
    {"newCmdLineParser", cmdLineParserCreate},

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

void extendCmdLine(lua_State* plua_state) 
{
    LuaRegCombUtilLib::getInstance().addRegArray((LuaReg*)cmdline_lib);
    luaCreateMeta(plua_state, kCmdLineParserHandle, (LuaReg*)cmdlineparser_obj_lib);
}
    
} //util