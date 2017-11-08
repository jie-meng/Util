#include "luaextend.hpp"
#include "lexicalcast.hpp"
#include "thread.hpp"
#include "file.hpp"
#include "net.hpp"
#include "lua/src/lua.hpp"
#include "lua/extend/lutillib.hpp"

namespace util
{

using namespace std;

const string kLuaExtendTag = "LuaExtend";
    
    //LuaRegComb
void LuaRegComb::addRegArray(LuaReg* reg)
{
    lib_reg_comb_.pop_back();
    while (reg->name != 0)
    {
        lib_reg_comb_.push_back(LuaReg(reg->name, reg->func));
        ++reg;
    }
    lib_reg_comb_.push_back(LuaReg());
}

void luaCreateMeta(lua_State* plua_state, const std::string& handleName, LuaReg* lr)
{
    luaL_newmetatable(plua_state, handleName.c_str());  /* create metatable for specified handles */
    lua_pushvalue(plua_state, -1);  /* push metatable */
    lua_setfield(plua_state, -2, "__index");  /* metatable.__index = metatable */
    luaL_setfuncs(plua_state, (luaL_Reg*)lr, 0);  /* add file methods to new metatable */
    lua_pop(plua_state, 1);  /* pop new metatable */
}

//LuaExtender
void LuaExtender::addLib(const std::string& name, LuaCFunc lib_create_func)
{
    libs_.push_back(std::make_pair(name, lib_create_func));
}

void LuaExtender::openLibs(lua_State* plua_state)
{
    std::vector<LuaLib>::iterator it;
    for (it = libs_.begin(); it != libs_.end(); ++it)
    {
        luaL_requiref(plua_state, (*it).first.c_str(), (*it).second, 1);
        lua_pop(plua_state, 1);  /* remove lib */
    }
}

class LuaUtilLibsExtender
{
public:
    SINGLETON(LuaUtilLibsExtender)
    void openlibs(lua_State* plua_state) { lua_extender_.openLibs(plua_state); }
    void addLib(const std::string& name, LuaCFunc lib_create_func)
    {
        lua_extender_.addLib(name, lib_create_func);
    }
    void resetLibs()
    {
        lua_extender_.clearLibs();
        addUtilExtendLibs();
    }
private:
    LuaUtilLibsExtender()
    {
        addUtilExtendLibs();
    }
    ~LuaUtilLibsExtender() {}

    void addUtilExtendLibs()
    {
        addLib(kLuaExtendLibUtil, lualibUtilCreate);
    }
private:
    LuaExtender lua_extender_;
private:
    DISALLOW_COPY_AND_ASSIGN(LuaUtilLibsExtender)
};

void luaCreateLib(lua_State* plua_state, LuaReg* lr)
{
    luaL_newlib(plua_state, (luaL_Reg*)lr);
}

void openUtilExtendLibs(lua_State* plua_state)
{
    LuaUtilLibsExtender::getInstance().openlibs(plua_state);
}

void addUtilExtendLib(const std::string& name, LuaCFunc lib_create_func)
{
    LuaUtilLibsExtender::getInstance().addLib(name, lib_create_func);
}

//LuaExtendCmdLine
LuaExtendCmdLine::LuaExtendCmdLine(const std::string& prompt,
               const std::string& quit_cmd,
               bool is_case_sensitive) :
                   LuaCmdLine(prompt, quit_cmd, is_case_sensitive)
{
    openUtilExtendLibs(getState());
}

LuaExtendCmdLine::~LuaExtendCmdLine()
{}

void LuaExtendCmdLine::clear()
{
    LuaCmdLine::clear();
    openUtilExtendLibs(getState());
}

//utilLuaExecutor
void luaExecuteThread(std::string file)
{
    LuaState ls;
    openUtilExtendLibs(ls.getState());
    int ret = ls.parseFile(file);
    if (0 != ret)
        printLine(ls.getError());
#ifdef _PLATFORM_WINDOWS_
    //_exit(0);
    exit(0);
#else
    exit(0);
#endif
}

void luaExecuteCmder(DgramSocket* pds, const std::string& ip, unsigned short port, const std::string& cmd)
{
    if (!pds)
        exit(0);
    pds->sendTo(cmd.c_str(), cmd.length(), ip, port);
}

void luaExecutor(int argc, char* argv[])
{
    if (1 == argc)
    {
        LuaExtendCmdLine lc;
        lc.start();
    }
    else if (2 == argc)
    {
        LuaState ls;
        openUtilExtendLibs(ls.getState());

        int ret = -1;
        if (isPathFile(argv[1])) {
            ret = ls.parseFile(argv[1]);    
        } else {
            ret = ls.parseLine(argv[1]);
        }

        if (0 != ret)
            printLine(ls.getError());
    }
    else
    {
        unsigned short port = lexicalCastDefault<unsigned short>(argv[2], 9901);
        std::string exit_cmd = "exit";

        if (argc > 3)
            exit_cmd = argv[3];

        DgramSocket ds(Family_IPv4);

        Thread td(UtilBind(luaExecuteThread, argv[1]));
        CmdLine cl(UtilBind(luaExecuteCmder, &ds, "127.0.0.1", port, _1), "", exit_cmd);

        td.start();
        cl.start();

        td.kill();
    }
}

void luaExtendError(lua_State* plua_state, const std::string& lib, const std::string& func, const std::string& msg)
{
    string message = strFormat("[%s - %s.%s] %s", kLuaExtendTag.c_str(), lib.c_str(), func.c_str(), msg.c_str());
    luaError(plua_state, message);
}

void luaExtendAssert(lua_State* plua_state, const std::string& lib, const std::string& func, bool assertion, const std::string& msg)
{
    string message = strFormat("[%s - %s.%s] %s", kLuaExtendTag.c_str(), lib.c_str(), func.c_str(), msg.c_str());
    luaAssert(plua_state, assertion, message);
}


} // namespace util
