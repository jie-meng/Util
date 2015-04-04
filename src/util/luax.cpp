#include "base.hpp"
#include <map>
#include <set>
#include "luaextend.hpp"
#include "thread.hpp"
#include "lua/src/lua.hpp"

namespace util
{

//get operate
double luaGetDouble(lua_State* plua_state, int index)
{
    //return luaL_checknumber(plua_state, index);
    return lua_tonumber(plua_state, index);
}

double luaGetDouble(lua_State* plua_state, int index, double default_num)
{
    return lua_isnumber(plua_state, index) ? luaGetDouble(plua_state, index) : default_num;
}

int luaGetInteger(lua_State* plua_state, int index)
{
    //return luaL_checkinteger(plua_state, index);
    return lua_tointeger(plua_state, index);
}

int luaGetInteger(lua_State* plua_state, int index, int default_int)
{
    return lua_isnumber(plua_state, index) ? luaGetInteger(plua_state, index) : default_int;
}

//unsigned int luaGetUInteger(lua_State* plua_state, int index)
//{
//    //return luaL_checkunsigned(plua_state, index);
//    //return lua_tounsigned(plua_state, index);
//}

//unsigned int luaGetUInteger(lua_State* plua_state, int index, unsigned int default_uint)
//{
//    return lua_isnumber(plua_state, index) ? luaGetInteger(plua_state, index) : default_uint;
//}

std::string luaGetString(lua_State* plua_state, int index)
{
    //return std::string(luaL_checkstring(plua_state, index));
    const char* result = lua_tostring(plua_state, index);
    if (!result)
        return "";

    return std::string(lua_tostring(plua_state, index));
}

std::string luaGetString(lua_State* plua_state, int index, const std::string& default_str)
{
    return lua_isstring(plua_state, index) ? luaGetString(plua_state, index) : default_str.c_str();
}

bool luaGetBoolean(lua_State* plua_state, int index)
{
    return bool(lua_toboolean(plua_state, index));
}

bool luaGetBoolean(lua_State* plua_state, int index, bool default_bool)
{
    return lua_isboolean(plua_state, index) ? luaGetBoolean(plua_state, index) : default_bool;
}

void* luaGetLightUserData(lua_State* plua_state, int index)
{
    return lua_touserdata(plua_state, index);
}

void* luaGetLightUserData(lua_State* plua_state, int index, void* default_data)
{
    return lua_isuserdata(plua_state, index) ? luaGetLightUserData(plua_state, index) : default_data;
}

any luaGetAny(lua_State* plua_state, int index)
{
    any a;
    switch (luaGetType(plua_state, index))
    {
        case LuaBoolean:
            a = luaGetBoolean(plua_state, index);
            break;
        case LuaLightUserData:
            a = luaGetLightUserData(plua_state, index);
            break;
        case LuaNumber:
            a = luaGetDouble(plua_state, index);
            break;
        case LuaString:
            a = luaGetString(plua_state, index);
            break;
        default:
            break;
    }
    return a;
}

//set operate
void luaPushDouble(lua_State* plua_state, double double_val)
{
    lua_pushnumber(plua_state, double_val);
}

void luaPushInteger(lua_State* plua_state, int int_val)
{
    lua_pushinteger(plua_state, int_val);
}

//void luaPushUInteger(lua_State* plua_state, unsigned int uint_val)
//{
//    lua_pushunsigned(plua_state, uint_val);
//}

void luaPushString(lua_State* plua_state, const std::string& str_val)
{
    lua_pushstring(plua_state, str_val.c_str());
}

void luaPushNil(lua_State* plua_state)
{
    lua_pushnil(plua_state);
}

void luaPushBoolean(lua_State* plua_state, bool boolean)
{
    lua_pushboolean(plua_state, boolean);
}

void luaPushLightUserData(lua_State* plua_state, void* p)
{
    lua_pushlightuserdata(plua_state, p);
}

void luaPushAny(lua_State* plua_state, const any& a)
{
    if (a.type() == typeid(bool))
    {
        luaPushBoolean(plua_state, any_cast<bool>(a));
    }
    else if (a.type() == typeid(void*))
    {
        luaPushLightUserData(plua_state, any_cast<void*>(a));
    }
    else if (a.type() == typeid(double))
    {
        luaPushDouble(plua_state, any_cast<double>(a));
    }
    else if (a.type() == typeid(std::string))
    {
        luaPushString(plua_state, any_cast<std::string>(a));
    }
    else if (a.type() == typeid(float))
    {
        luaPushDouble(plua_state, (double)any_cast<float>(a));
    }
    else if (a.type() == typeid(int))
    {
        luaPushInteger(plua_state, any_cast<int>(a));
    }
    else if (a.type() == typeid(short))
    {
        luaPushInteger(plua_state, (int)any_cast<short>(a));
    }
    else if (a.type() == typeid(long))
    {
        luaPushInteger(plua_state, (int)any_cast<long>(a));
    }
    else if (a.type() == typeid(uint8_t))
    {
        luaPushInteger(plua_state, (int)any_cast<uint8_t>(a));
    }
    else if (a.type() == typeid(uint16_t))
    {
        luaPushInteger(plua_state, (int)any_cast<uint16_t>(a));
    }
    else if (a.type() == typeid(uint32_t))
    {
        luaPushInteger(plua_state, (int)any_cast<uint32_t>(a));
    }
    else
    {
        luaPushNil(plua_state);
    }
}

//table operate
void luaPushTable(lua_State* plua_state, const std::vector< std::pair<any, any> >& key_value_vec)
{
    lua_newtable(plua_state);

    std::vector< std::pair<any, any> >::const_iterator it;
    for (it = key_value_vec.begin(); it != key_value_vec.end(); ++it)
    {
        luaPushAny(plua_state, it->first);
        luaPushAny(plua_state, it->second);
        lua_settable(plua_state, -3);
    }
}

//other operate
LuaType luaGetType(lua_State* plua_state, int index)
{
    return (LuaType)lua_type(plua_state, index);
}

std::string luaGetTypeName(lua_State* plua_state, LuaType type)
{
    return lua_typename(plua_state, type);
}

void luaPop(lua_State* plua_state, int index)
{
    lua_pop(plua_state, index);
}

int luaGetTop(lua_State* plua_state)
{
    return lua_gettop(plua_state);
}

void luaGetGlobal(lua_State* plua_state, const std::string& name)
{
    lua_getglobal(plua_state, name.c_str());
}

int luaCallFunc(lua_State* plua_state, int nargs, int nrets)
{
    return lua_pcall(plua_state, nargs, nrets, 0);
}

void luaAssert(lua_State* plua_state, bool assertion, const std::string& str)
{
    if (!plua_state)
        luaL_error(plua_state, "plua_state is null");

    //if(assertion)
        //return lua_gettop(plua_state);

    if (!assertion)
        luaL_error(plua_state, "%s", str.c_str());
}

void luaError(lua_State* plua_state, const std::string& str)
{
    luaL_error(plua_state, "%s", str.c_str());
}

//luaGetError
std::string luaGetError(lua_State* plua_state, int err)
{
    std::string error_str("");
    if (0 == err)
        return error_str;

    std::string err_type;
    switch (err)
    {
        case LUA_ERRSYNTAX: //compile-time error
            err_type = strFormat("syntax error during pre-compilation");
            break;
        case LUA_ERRMEM: //memory error
            err_type = strFormat("memory allocation error");
            break;
        case LUA_ERRRUN: //runtime-time error
            err_type = strFormat("runtime error");
            break;
        case LUA_YIELD: //thread suspend error
            err_type = strFormat("thread has been suspended");
            break;
        case LUA_ERRERR: //error while running
            err_type = strFormat("error while running the error handler function");
            break;
        default:
            err_type = strFormat("unknown");
            break;
    }

    error_str = strFormat("error(%s) %s", err_type.c_str(), lua_tostring(plua_state, -1));
    luaPop(plua_state, 1);

    return error_str;
}

//luaParseLine
int luaParseLine(lua_State* plua_state, const std::string& line, std::string& error_str)
{
    error_str = "";
    if (0 == plua_state)
        return -1;

    int err = luaL_loadbuffer(plua_state, line.c_str(), line.length(), "line");
    if (0 != err)
    {
        error_str = luaGetError(plua_state, err);
        return err;
    }

    err = lua_pcall(plua_state, 0, 0, 0);
    error_str = luaGetError(plua_state, err);

    return err;
}

//luaParseFile
int luaParseFile(lua_State* plua_state, const std::string& file, std::string& error_str)
{
    error_str = "";
    if (0 == plua_state)
        return -1;

    int err = luaL_loadfile(plua_state, file.c_str());
    if (0 != err)
    {
        error_str = luaGetError(plua_state, err);
        return err;
    }
    err = lua_pcall(plua_state, 0, LUA_MULTRET, 0);
    error_str = luaGetError(plua_state, err);
    return err;
}

//LuaHeapRecycler
typedef std::map<void*, LuaHeapObjectInfo> HeapMap;
typedef std::set<std::string> IncludeSet;

struct LuaHeapRecycler::LuaHeapRecyclerImpl
{
    void addHeapObject(void* p, LuaHeapObjectInfo lua_heap_object_info);
    void removeHeapObject(void* p);
    void clear();
    void clear(TypeInfo typeinfo);
    bool include(const std::string& file);

    HeapMap heap_map_;
    IncludeSet include_set_;
    Mutex mutex_;
};

void LuaHeapRecycler::LuaHeapRecyclerImpl::addHeapObject(void* p, LuaHeapObjectInfo lua_heap_object_info)
{
    Synchronize sc(mutex_);
    if (!p)
        return;
    heap_map_.insert(std::make_pair(p, lua_heap_object_info));
}

void LuaHeapRecycler::LuaHeapRecyclerImpl::removeHeapObject(void* p)
{
    Synchronize sc(mutex_);
    if (!p)
        return;
    HeapMap::iterator it = heap_map_.find(p);
    if (it != heap_map_.end())
        heap_map_.erase(p);
}

bool LuaHeapRecycler::LuaHeapRecyclerImpl::include(const std::string& file)
{
    return include_set_.insert(file).second;
}

void LuaHeapRecycler::LuaHeapRecyclerImpl::clear()
{
    Synchronize sc(mutex_);
    for (HeapMap::iterator it = heap_map_.begin(); it != heap_map_.end(); ++it)
    {
        //delete it
        it->second.recycler(it->first);
    }
    heap_map_.clear();
    include_set_.clear();
}

void LuaHeapRecycler::LuaHeapRecyclerImpl::clear(TypeInfo typeinfo)
{
    Synchronize sc(mutex_);
    for (HeapMap::iterator it = heap_map_.begin(); it != heap_map_.end();)
    {
        if (it->second.typeinfo == typeinfo)
        {
            it->second.recycler(it->first);
            heap_map_.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

LuaHeapRecycler::LuaHeapRecycler() :
    pimpl_(new LuaHeapRecyclerImpl())
{}

LuaHeapRecycler::~LuaHeapRecycler()
{}

void LuaHeapRecycler::addHeapObject(void* p, LuaHeapObjectInfo lua_heap_object_info)
{
    pimpl_->addHeapObject(p, lua_heap_object_info);
}

void LuaHeapRecycler::removeHeapObject(void* p)
{
    pimpl_->removeHeapObject(p);
}

bool LuaHeapRecycler::include(const std::string& file)
{
    return pimpl_->include(file);
}

void LuaHeapRecycler::clear()
{
    pimpl_->clear();
}

void LuaHeapRecycler::clear(TypeInfo typeinfo)
{
    pimpl_->clear(typeinfo);
}

//LuaHeapRecyclerManager
struct LuaHeapRecyclerManager::LuaHeapRecyclerManagerImpl
{
    std::map<lua_State*, LuaHeapRecycler*> state_map_;
    Mutex lua_heap_recycler_mutex_;
};

LuaHeapRecyclerManager::LuaHeapRecyclerManager() :
    pimpl_(new LuaHeapRecyclerManagerImpl())
{}

LuaHeapRecyclerManager::~LuaHeapRecyclerManager()
{}

void LuaHeapRecyclerManager::addHeapObject(lua_State* plua_state, void* p, LuaHeapObjectInfo lua_heap_object_info)
{
    Synchronize sc(pimpl_->lua_heap_recycler_mutex_);
    std::map<lua_State*, LuaHeapRecycler*>::iterator it = pimpl_->state_map_.find(plua_state);
    if (it != pimpl_->state_map_.end())
        (it->second)->addHeapObject(p, lua_heap_object_info);
}

void LuaHeapRecyclerManager::removeHeapObject(lua_State* plua_state, void* p)
{
    Synchronize sc(pimpl_->lua_heap_recycler_mutex_);
    std::map<lua_State*, LuaHeapRecycler*>::iterator it = pimpl_->state_map_.find(plua_state);
    if (it != pimpl_->state_map_.end())
        (it->second)->removeHeapObject(p);
}

bool LuaHeapRecyclerManager::include(lua_State* plua_state, const std::string& file)
{
    Synchronize sc(pimpl_->lua_heap_recycler_mutex_);
    std::map<lua_State*, LuaHeapRecycler*>::iterator it = pimpl_->state_map_.find(plua_state);
    if (it != pimpl_->state_map_.end())
        return (it->second)->include(file);

    return false;
}

void LuaHeapRecyclerManager::addState(lua_State* plua_state, LuaHeapRecycler* ph)
{
    Synchronize sc(pimpl_->lua_heap_recycler_mutex_);
    pimpl_->state_map_.insert(std::make_pair(plua_state, ph));
}

void LuaHeapRecyclerManager::removeState(lua_State* plua_state)
{
    Synchronize sc(pimpl_->lua_heap_recycler_mutex_);
    pimpl_->state_map_.erase(plua_state);
}

void LuaHeapRecyclerManager::clear(lua_State* plua_state)
{
    Synchronize sc(pimpl_->lua_heap_recycler_mutex_);
    std::map<lua_State*, LuaHeapRecycler*>::iterator it = pimpl_->state_map_.find(plua_state);
    if (it != pimpl_->state_map_.end())
        (it->second)->clear();
}

void LuaHeapRecyclerManager::clear(lua_State* plua_state, TypeInfo typeinfo)
{
    Synchronize sc(pimpl_->lua_heap_recycler_mutex_);
    std::map<lua_State*, LuaHeapRecycler*>::iterator it = pimpl_->state_map_.find(plua_state);
    if (it != pimpl_->state_map_.end())
        (it->second)->clear(typeinfo);
}

//extend basic functions
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

static int include(lua_State* plua_state)
{
    //do not include same file
    if (!LuaHeapRecyclerManager::getInstance().include(plua_state, luaGetString(plua_state, 1)))
        return 0;

    return lua_dofile_export(plua_state);
}

static int run(lua_State* plua_state)
{
    //clear heap before run
    LuaHeapRecyclerManager::getInstance().clear(plua_state);

    return lua_dofile_export(plua_state);
}

//LuaState
LuaState::LuaState() :
    plua_state_(0)
{
    init();
}

LuaState::~LuaState()
{
    cleanup();
}

bool LuaState::init()
{
    if (plua_state_)
        return false;

    plua_state_ = luaL_newstate();
    loadLibs();
    extendBasicFunctions();

    LuaHeapRecyclerManager::getInstance().addState(plua_state_, &lua_heap_recycler_);

    return true;
}

void LuaState::cleanup()
{
    clearHeap();

    if (plua_state_)
        lua_close(plua_state_);

    LuaHeapRecyclerManager::getInstance().removeState(plua_state_);
    plua_state_ = 0;
}

bool LuaState::loadLibs()
{
    if (0 != plua_state_)
    {
        luaL_openlibs(plua_state_);
        return true;
    }
    else
    {
        return false;
    }
}

bool LuaState::reset()
{
    cleanup();
    return init();
}

void LuaState::extendBasicFunctions()
{
    registerFunction("sleep", sleep);
    registerFunction("msleep", msleep);
    registerFunction("include", include);
    registerFunction("run", run);
}

int LuaState::parseLine(const std::string& line)
{
    return luaParseLine(plua_state_, line, error_str);
}

int LuaState::parseFile(const std::string& file)
{
    return luaParseFile(plua_state_, file, error_str);
}

void LuaState::clearHeap()
{
    lua_heap_recycler_.clear();
}

void LuaState::registerFunction(const std::string& func_name, LuaCFunc lua_reg_func)
{
    lua_register(plua_state_, func_name.c_str(), lua_reg_func);
}

//LuaCmdLine
LuaCmdLine::LuaCmdLine(const std::string& prompt,
                       const std::string& quit_cmd,
                       bool is_case_sensitive) :
                           CommonCmdLine(prompt, quit_cmd, is_case_sensitive)
{}

LuaCmdLine::~LuaCmdLine()
{}

void LuaCmdLine::registerFunction(const std::string& func_name, LuaCFunc lua_reg_func)
{
    lua_state_.registerFunction(func_name, lua_reg_func);
}

void LuaCmdLine::process(const std::string& cmd)
{
    if(strIsEqual(cmd, "clearheap", isCaseSensitive()))
    {
        clearHeap();
        return;
    }
    else if(strIsEqual(cmd, "clear", isCaseSensitive()))
    {
        clear();
        return;
    }
    else
    {
        if(0 != lua_state_.parseLine(cmd))
            printLine(lua_state_.getError());
    }
}

int LuaCmdLine::parseFile(const std::string& file)
{
    int res = lua_state_.parseFile(file);
    if(0 != res)
        printLine(lua_state_.getError());

    clear();

    return res;
}

void LuaCmdLine::clear()
{
    lua_state_.reset();
}

} // namespace util
