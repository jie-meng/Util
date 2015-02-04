#include "luaextend.hpp"
#include "lua/src/lua.hpp"
#include "lexicalcast.hpp"
#include "thread.hpp"
#include "net.hpp"
#include "lua/extend/lmemorylib.hpp"
#include "lua/extend/lregexlib.hpp"
#include "lua/extend/lnetlib.hpp"
#include "lua/extend/lthreadlib.hpp"
#include "lua/extend/lfilelib.hpp"
#include "lua/extend/lprocesslib.hpp"
#include "lua/extend/lcsvlib.hpp"

namespace util
{

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
        lua_extender_.addLib(kLuaExtendLibMemory, lualibMemoryCreate);
        lua_extender_.addLib(kLuaExtendLibFile, lualibFileCreate);
        lua_extender_.addLib(kLuaExtendLibRegex, lualibRegexCreate);
        lua_extender_.addLib(kLuaExtendLibNet, lualibNetCreate);
        lua_extender_.addLib(kLuaExtendLibThread, lualibThreadCreate);
        lua_extender_.addLib(kLuaExtendLibProcess, lualibProcessCreate);
        lua_extender_.addLib(kLuaExtendLibCsv, lualibCsvCreate);
    }
private:
    LuaExtender lua_extender_;
private:
    DISALLOW_COPY_AND_ASSIGN(LuaUtilLibsExtender)
};

void luaCreateLib(lua_State* plua_state, u_luaL_Reg* lr)
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
    _exit(0);
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
        int ret = ls.parseFile(argv[1]);
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

} // namespace util
