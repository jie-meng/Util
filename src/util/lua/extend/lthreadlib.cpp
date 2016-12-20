#include "lthreadlib.hpp"
#include <vector>
#include "util/luaextend.hpp"
#include "util/thread.hpp"
#include "util/any.hpp"

namespace util
{

static void threadFunc(std::string file, std::string func, std::vector<any> args)
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

    for (size_t i=0; i<args.size(); ++i)
        luaPushAny(ls.getState(), args[i]);

    err = luaCallFunc(ls.getState(), args.size(), 0);
    if (0 != err)
    {
        printLine(luaGetError(ls.getState(), err));
        return;
    }

    luaPop(ls.getState(), -1);
}

static int create(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibThread, "create", luaGetTop(plua_state) >= 2, "file and threadfunc needed");
    luaExtendAssert(plua_state, kLuaExtendLibThread, "create",
        LuaString == luaGetType(plua_state, 1) && LuaString == luaGetType(plua_state, 2), "file and threadfunc must be string type");

    std::string file = luaGetString(plua_state, 1, "");
    std::string func = luaGetString(plua_state, 2, "");

    std::vector<any> args;
    for (int i=3; i<=luaGetTop(plua_state); ++i)
        args.push_back(luaGetAny(plua_state, i));

    Thread* pthread = new Thread(UtilBind(threadFunc, file, func, args));
    LuaHeapRecyclerManager::getInstance().addHeapObject<Thread>(plua_state, (void*)pthread);

    luaPushLightUserData(plua_state, (void*)pthread);

    return 1;
}

static int destrory(lua_State* plua_state)
{
    Thread* pthread = static_cast<Thread*>(luaGetLightUserData(plua_state, 1, 0));

    LuaHeapRecyclerManager::getInstance().removeHeapObject(plua_state, (void*)pthread);

    if (pthread)
        delete pthread;

    return 0;
}

static int start(lua_State* plua_state)
{
    Thread* pthread = static_cast<Thread*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibThread, "start", pthread, "null pointer");

    luaPushBoolean(plua_state, pthread->start(luaGetInteger(plua_state, 2, 0)));

    return 1;
}

static int join(lua_State* plua_state)
{
    Thread* pthread = static_cast<Thread*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibThread, "join", pthread, "null pointer");

    luaPushBoolean(plua_state, pthread->join());
    return 1;
}

static int kill(lua_State* plua_state)
{
    Thread* pthread = static_cast<Thread*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibThread, "kill", pthread, "null pointer");

    pthread->kill();
    return 0;
}

static int getThreadId(lua_State* plua_state)
{
    luaPushInteger(plua_state, getCurrentThreadId());
    return 1;
}

static int mutexCreate(lua_State* plua_state)
{
    Mutex* pmutex = new Mutex();
    LuaHeapRecyclerManager::getInstance().addHeapObject<Mutex>(plua_state, (void*)pmutex);

    luaPushLightUserData(plua_state, (void*)pmutex);

    return 1;
}

static int mutexDestroy(lua_State* plua_state)
{
    Mutex* pmutex = static_cast<Mutex*>(luaGetLightUserData(plua_state, 1, 0));
    LuaHeapRecyclerManager::getInstance().removeHeapObject(plua_state, (void*)pmutex);

    if (pmutex)
        delete pmutex;

    return 0;
}

static int mutexLock(lua_State* plua_state)
{
    Mutex* pmutex = static_cast<Mutex*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibThread, "mutexLock", pmutex, "null pointer");

    pmutex->lock();
    return 0;
}

static int mutexUnlock(lua_State* plua_state)
{
    Mutex* pmutex = static_cast<Mutex*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibThread, "mutexUnlock", pmutex, "null pointer");

    pmutex->unlock();
    return 0;
}

static int lockCreate(lua_State* plua_state)
{
    Lock* plock = new Lock();
    LuaHeapRecyclerManager::getInstance().addHeapObject<Lock>(plua_state, (void*)plock);

    luaPushLightUserData(plua_state, (void*)plock);

    return 1;
}

static int lockDestroy(lua_State* plua_state)
{
    Lock* plock = static_cast<Lock*>(luaGetLightUserData(plua_state, 1, 0));
    LuaHeapRecyclerManager::getInstance().removeHeapObject(plua_state, (void*)plock);

    if (plock)
        delete plock;

    return 0;
}

static int lockWait(lua_State* plua_state)
{
    Lock* plock = static_cast<Lock*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibThread, "lockWait", plock, "null pointer");

    plock->wait(luaGetBoolean(plua_state, 2, true));

    return 0;
}

static int lockTimedWait(lua_State* plua_state)
{
    Lock* plock = static_cast<Lock*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibThread, "lockTimedWait", plock, "null pointer");

    luaPushBoolean(plua_state, plock->timedWait(luaGetInteger(plua_state, 2, 2000),
                                                 luaGetBoolean(plua_state, 3, true)));
    return 1;
}

static int lockNotify(lua_State* plua_state)
{
    Lock* plock = static_cast<Lock*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibThread, "lockNotify", plock, "null pointer");

    plock->notify();

    return 0;
}

static const u_luaL_Reg thread_lib[] =
{
    {"create", create},
    {"destroy", destrory},
    {"start", start},
    {"join", join},
    {"kill", kill},

    {"getThreadId", getThreadId},

    {"mutexCreate", mutexCreate},
    {"mutexDestroy", mutexDestroy},
    {"mutexLock", mutexLock},
    {"mutexUnlock", mutexUnlock},

    {"lockCreate", lockCreate},
    {"lockDestroy", lockDestroy},
    {"lockWait", lockWait},
    {"lockTimedWait", lockTimedWait},
    {"lockNotify", lockNotify},

    {0, 0}
};

/*
** Open regex library
*/
int lualibThreadCreate(lua_State* plua_state) {

    luaCreateLib(plua_state, (u_luaL_Reg*)thread_lib);
    return 1;
}

} // namespace util
