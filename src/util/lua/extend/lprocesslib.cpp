#include "lprocesslib.hpp"
#include "util/luaextend.hpp"
#include "util/process.hpp"
#include "util/any.hpp"

namespace util
{

static int create(lua_State* plua_state)
{
    Process* pprocess = new Process();

    //LuaHeapRecyclerManager::getInstance().addHeapObject(plua_state, (void*)pprocess, deleteVoid<Process>);
    LuaHeapRecyclerManager::getInstance().addHeapObject<Process>(plua_state, (void*)pprocess);

    luaPushLightUserData(plua_state, (void*)pprocess);

    return 1;
}

static int destrory(lua_State* plua_state)
{
    Process* pprocess = static_cast<Process*>(luaGetLightUserData(plua_state, 1, 0));

    LuaHeapRecyclerManager::getInstance().removeHeapObject(plua_state, (void*)pprocess);

    if (pprocess)
        delete pprocess;

    return 0;
}

void outputFunc(std::string file, std::string func, const std::string& str, std::vector<any> args)
{
    LuaState ls;
    openUtilExtendLibs(ls.getState());

    int err = ls.parseFile(file);
    if (0 != err)
    {
        printLine(luaGetError(ls.getState(), err));
        return;
    }

    luaGetGlobal(ls.getState(), func);

    luaPushString(ls.getState(), str);
    for (size_t i=0; i<args.size(); ++i)
        luaPushAny(ls.getState(), args[i]);

    err = luaCallFunc(ls.getState(), 1 + args.size() , 0);
    if (0 != err)
    {
        printLine(luaGetError(ls.getState(), err));
        return;
    }

    luaPop(ls.getState(), -1);
}

static int start(lua_State* plua_state)
{
    Process* pprocess = static_cast<Process*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibProcess, "start", pprocess,
        "null pointer");

    std::vector<any> args;
    for (int i=8; i<=luaGetTop(plua_state); ++i)
        args.push_back(luaGetAny(plua_state, i));

    luaPushBoolean(plua_state, pprocess->create(luaGetString(plua_state, 2, ""),
                                                luaGetString(plua_state, 3, ""),
                                                luaGetBoolean(plua_state, 4, false),
                                                luaGetBoolean(plua_state, 5, false),
                                                UtilBind(outputFunc,
                                                        luaGetString(plua_state, 6, ""),
                                                        luaGetString(plua_state, 7, ""),
                                                        _1,
                                                        args
                                                        )
                                                ));
    return 1;
}

static int kill(lua_State* plua_state)
{
    Process* pprocess = static_cast<Process*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibProcess, "kill", pprocess,
        "null pointer");

    pprocess->kill();
    return 0;
}

static int input(lua_State* plua_state)
{
    Process* pprocess = static_cast<Process*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibProcess, "input", pprocess,
        "null pointer");

    luaPushBoolean(plua_state, pprocess->input(luaGetString(plua_state, 2, "")));
    return 1;
}

//execute process and wait it end, returns process exit_code (fail : -1)
static int executeProcess(lua_State* plua_state)
{
    luaPushInteger(plua_state, executeProcess(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, "")));
    return 1;
}

//execute process and do not wait, returns immidiately.
static int executeProcessAsyn(lua_State* plua_state)
{
    luaPushBoolean(plua_state, executeProcessAsyn(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, "")));
    return 1;
}

static const u_luaL_Reg process_lib[] =
{
    {"create", create},
    {"destroy", destrory},
    {"start", start},
    {"kill", kill},
    {"input", input},

    {"executeProcess", executeProcess},
    {"executeProcessAsyn", executeProcessAsyn},

    {0, 0}
};

int lualibProcessCreate(lua_State* plua_state)
{
    luaCreateLib(plua_state, (u_luaL_Reg*)process_lib);
    return 1;
}

} // namespace util
