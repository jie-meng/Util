#include "lthreadext.hpp"
#include <vector>
#include "util/luaextend.hpp"
#include "util/thread.hpp"
#include "util/any.hpp"

namespace util
{

const std::string kThreadHandle = "Thread*";
const std::string kMutexHandle = "Mutex*";
const std::string kLockHandle = "Lock*";

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

static int threadCreate(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "create", luaGetTop(plua_state) >= 2, "file and threadfunc needed");
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "create",
        LuaString == luaGetType(plua_state, 1) && LuaString == luaGetType(plua_state, 2), "file and threadfunc must be string type");

    std::string file = luaGetString(plua_state, 1, "");
    std::string func = luaGetString(plua_state, 2, "");

    std::vector<any> args;
    for (int i=3; i<=luaGetTop(plua_state); ++i)
        args.push_back(luaGetAny(plua_state, i));
    
    LuaObject<Thread>* p = luaNewEmptyObject<Thread>(plua_state, kThreadHandle);
    p->setData(new Thread(UtilBind(threadFunc, file, func, args)));

    return 1;
}

static int threadDestroy(lua_State* plua_state)
{
    return luaObjectDestroy<Thread>(plua_state, kThreadHandle);
}

static int threadStart(lua_State* plua_state)
{
    Thread* pthread = luaGetObjectData<Thread>(plua_state, kThreadHandle);
    luaPushBoolean(plua_state, pthread->start(luaGetInteger(plua_state, 2, 0)));

    return 1;
}

static int threadJoin(lua_State* plua_state)
{
    Thread* pthread = luaGetObjectData<Thread>(plua_state, kThreadHandle);
    pthread->join();
    return 0;
}

static int threadKill(lua_State* plua_state)
{
    Thread* pthread = luaGetObjectData<Thread>(plua_state, kThreadHandle);
    pthread->kill();
    return 0;
}

static int threadToString(lua_State* plua_state)
{
    return luaObjectToString<Thread>(plua_state, kThreadHandle);
}

static int getThreadId(lua_State* plua_state)
{
    luaPushInteger(plua_state, getCurrentThreadId());
    return 1;
}

static int sleep(lua_State* plua_state)
{
    util::sleep(luaGetInteger(plua_state, 1, 0));
    return 0;
}

static int msleep(lua_State* plua_state)
{
    util::msleep(luaGetInteger(plua_state, 1, 0));
    return 0;
}

static int mutexCreate(lua_State* plua_state)
{
    LuaObject<Mutex>* p = luaNewEmptyObject<Mutex>(plua_state, kMutexHandle);
    p->setData(new Mutex());
    
    return 1;
}

static int mutexDestroy(lua_State* plua_state)
{
    return luaObjectDestroy<Mutex>(plua_state, kMutexHandle);
}

static int mutexLock(lua_State* plua_state)
{
    Mutex* pmutex = luaGetObjectData<Mutex>(plua_state, kMutexHandle);
    pmutex->lock();
    
    return 0;
}

static int mutexUnlock(lua_State* plua_state)
{
    Mutex* pmutex = luaGetObjectData<Mutex>(plua_state, kMutexHandle);
    pmutex->unlock();
    return 0;
}

static int mutexToString(lua_State* plua_state)
{
    return luaObjectToString<Mutex>(plua_state, kMutexHandle);
}

static int lockCreate(lua_State* plua_state)
{
    LuaObject<Lock>* p = luaNewEmptyObject<Lock>(plua_state, kLockHandle);
    p->setData(new Lock());

    return 1;
}

static int lockDestroy(lua_State* plua_state)
{
    return luaObjectDestroy<Lock>(plua_state, kLockHandle);
}

static int lockWait(lua_State* plua_state)
{
    Lock* plock = luaGetObjectData<Lock>(plua_state, kLockHandle);   
    plock->wait(luaGetBoolean(plua_state, 2, true));

    return 0;
}

static int lockTimedWait(lua_State* plua_state)
{
    Lock* plock = luaGetObjectData<Lock>(plua_state, kLockHandle);
    luaPushBoolean(plua_state, plock->timedWait(luaGetInteger(plua_state, 2, 2000),
                                                 luaGetBoolean(plua_state, 3, true)));
    return 1;
}

static int lockNotify(lua_State* plua_state)
{
    Lock* plock = luaGetObjectData<Lock>(plua_state, kLockHandle);
    plock->notify();

    return 0;
}

static int lockToString(lua_State* plua_state)
{
    return luaObjectToString<Lock>(plua_state, kLockHandle);
}

static const LuaReg thread_lib[] =
{
    {"newThread", threadCreate},
    {"newMutex", mutexCreate},
    {"newLock", lockCreate},
    
    {"getThreadId", getThreadId},
    {"sleep", sleep},
    {"msleep", msleep},

    {0, 0}
};

static const LuaReg thread_obj_lib[] = {
    {"delete", threadDestroy},
    {"start", threadStart},
    {"join", threadJoin},
    {"kill", threadKill},
    {"__gc", threadDestroy},
    {"__tostring", threadToString},
    
    {0, 0}
};

static const LuaReg mutex_obj_lib[] = {
    {"delete", mutexDestroy},
    {"lock", mutexLock},
    {"unlock", mutexUnlock},
    {"__gc", mutexDestroy},
    {"__tostring", mutexToString},
    
    {0, 0}
};

static const LuaReg lock_obj_lib[] = {
    {"delete", lockDestroy},
    {"wait", lockWait},
    {"timedWait", lockTimedWait},
    {"notify", lockNotify},
    {"__gc", lockDestroy},
    {"__tostring", lockToString},
    
    {0, 0}
};

void extendThread(lua_State* plua_state) 
{
    LuaRegCombUtilLib::getInstance().addRegArray((LuaReg*)thread_lib);
    luaCreateMeta(plua_state, kThreadHandle, (LuaReg*)thread_obj_lib);
    luaCreateMeta(plua_state, kMutexHandle, (LuaReg*)mutex_obj_lib);
    luaCreateMeta(plua_state, kLockHandle, (LuaReg*)lock_obj_lib);
}

} // namespace util
