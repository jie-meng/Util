#include "lprocesslib.hpp"
#include "util/luaextend.hpp"
#include "util/process.hpp"
#include "util/any.hpp"

namespace util
{

const std::string kProcessHandle = "Process*";

static int createProcess(lua_State* plua_state)
{
    LuaObject<Process>* p = luaNewEmptyObject<Process>(plua_state, kProcessHandle);
    p->setData(new Process());

    return 1;
}

static int destroy(lua_State* plua_state)
{
    return luaObjectDestroy<Process>(plua_state, kProcessHandle);
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
    Process* pprocess = luaGetObjectData<Process>(plua_state, kProcessHandle);

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
    Process* pprocess = luaGetObjectData<Process>(plua_state, kProcessHandle);
    pprocess->kill();
    return 0;
}

static int input(lua_State* plua_state)
{
    Process* pprocess = luaGetObjectData<Process>(plua_state, kProcessHandle);
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

static int toString(lua_State* plua_state)
{
    return luaObjectToString<Process>(plua_state, kProcessHandle);
}

static const LuaReg process_lib[] =
{
    {"createProcess", createProcess},
        
    {"executeProcess", executeProcess},
    {"executeProcessAsyn", executeProcessAsyn},
        
    {0, 0}
};

static const LuaReg process_obj_lib[] = {
    {"destroy", destroy},
    {"start", start},
    {"kill", kill},
    {"input", input},
    {"__gc", destroy},
    {"__tostring", toString},
    
    {0, 0}
};

void extendProcess(lua_State* plua_state)
{
    LuaRegCombUtilLib::getInstance().addRegArray((LuaReg*)process_lib);
    luaCreateMeta(plua_state, kProcessHandle, (LuaReg*)process_obj_lib);
}

} // namespace util
