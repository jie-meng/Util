#ifndef UTIL_LUAX_HPP
#define UTIL_LUAX_HPP

#include <vector>
#include "cmdline.hpp"
#include "any.hpp"
#include "typeinfo.hpp"

struct lua_State;

namespace util
{

typedef int (*LuaCFunc)(lua_State*);

enum LuaType
{
    LuaNone = -1,
    LuaNil,
    LuaBoolean,
    LuaLightUserData,
    LuaNumber,
    LuaString,
    LuaTable,
    LuaFunction,
    LuaUserData,
    LuaThread,
    LuaNumTags
};

//get operate
double luaGetDouble(lua_State* plua_state, int index);
double luaGetDouble(lua_State* plua_state, int index, double default_num);
int luaGetInteger(lua_State* plua_state, int index);
int luaGetInteger(lua_State* plua_state, int index, int default_int);
//unsigned int luaGetUInteger(lua_State* plua_state, int index);
//unsigned int luaGetUInteger(lua_State* plua_state, int index, unsigned int default_uint);
std::string luaGetString(lua_State* plua_state, int index);
std::string luaGetString(lua_State* plua_state, int index, const std::string& default_str);
bool luaGetBoolean(lua_State* plua_state, int index);
bool luaGetBoolean(lua_State* plua_state, int index, bool default_bool);
void* luaGetLightUserData(lua_State* plua_state, int index);
void* luaGetLightUserData(lua_State* plua_state, int index, void* default_data);
any luaGetAny(lua_State* plua_state, int index);

//push operate
void luaPushDouble(lua_State* plua_state, double double_val);
void luaPushInteger(lua_State* plua_state, int int_val);
//void luaPushUInteger(lua_State* plua_state, unsigned int uint_val);
void luaPushString(lua_State* plua_state, const std::string& str_val);
void luaPushNil(lua_State* plua_state);
void luaPushBoolean(lua_State* plua_state, bool boolean);
void luaPushLightUserData(lua_State* plua_state, void* p);
void luaPushAny(lua_State* plua_state, const any& a);

//other operate
LuaType luaGetType(lua_State* plua_state, int index);
std::string luaGetTypeName(lua_State* plua_state, LuaType type);
void luaPop(lua_State* plua_state, int index);
int luaGetTop(lua_State* plua_state);
void luaGetGlobal(lua_State* plua_state, const std::string& name);
int luaCallFunc(lua_State* plua_state, int nargs, int nrets);
int luaAssert(lua_State* plua_state, bool assertion, const std::string& str);

std::string luaGetError(lua_State* plua_state, int err);
int luaParseLine(lua_State* plua_state, const std::string& line, std::string& error_str);
int luaParseFile(lua_State* plua_state, const std::string& file, std::string& error_str);

//LuaHeapRecycler
typedef void (*RecycleFunc)(void*);

struct LuaHeapObjectInfo
{
    LuaHeapObjectInfo(TypeInfo ti, RecycleFunc r)
    {
        typeinfo = ti;
        recycler = r;
    }
    TypeInfo typeinfo;
    RecycleFunc recycler;
};

class LuaHeapRecycler
{
public:
    LuaHeapRecycler();
    ~LuaHeapRecycler();

    void addHeapObject(void* p, LuaHeapObjectInfo lua_heap_object_info);

    template<typename T>
    void addHeapObject(void* p);

    void removeHeapObject(void* p);
    bool include(const std::string& file);
    void clear();
    void clear(TypeInfo typeinfo);
private:
    struct LuaHeapRecyclerImpl;
    UtilAutoPtr<LuaHeapRecyclerImpl> pimpl_;
private:
    DISALLOW_COPY_AND_ASSIGN(LuaHeapRecycler)
};

template<typename T>
void LuaHeapRecycler::addHeapObject(void* p)
{
    addHeapObject(p, LuaHeapObjectInfo(typeid(T), deleteVoid<T>));
}

class LuaHeapRecyclerManager
{
public:
    SINGLETON(LuaHeapRecyclerManager)

    void addHeapObject(lua_State* plua_state, void* p, LuaHeapObjectInfo lua_heap_object_info);

    template<typename T>
    void addHeapObject(lua_State* plua_state, void* p);

    void removeHeapObject(lua_State* plua_state, void* p);
    bool include(lua_State* plua_state, const std::string& file);
    void clear(lua_State* plua_state);
    void clear(lua_State* plua_state, TypeInfo typeinfo);
    void addState(lua_State* plua_state, LuaHeapRecycler* ph);
    void removeState(lua_State* plua_state);
private:
    LuaHeapRecyclerManager();
    ~LuaHeapRecyclerManager();
private:
    struct LuaHeapRecyclerManagerImpl;
    UtilAutoPtr<LuaHeapRecyclerManagerImpl> pimpl_;
private:
    DISALLOW_COPY_AND_ASSIGN(LuaHeapRecyclerManager)
};

template<typename T>
void LuaHeapRecyclerManager::addHeapObject(lua_State* plua_state, void* p)
{
    addHeapObject(plua_state, p, LuaHeapObjectInfo(typeid(T), deleteVoid<T>));
}

//LuaState
class LuaState
{
public:
    LuaState();
    ~LuaState();
    inline lua_State* getState() const { return plua_state_; }
    inline std::string getError() const { return error_str; }
    void registerFunction(const std::string& func_name, LuaCFunc lua_reg_func);
    int parseLine(const std::string& line);
    int parseFile(const std::string& file);
    void clearHeap();
    bool reset();
private:
    bool init();
    void cleanup();
    bool loadLibs();
    void extendBasicFunctions();
private:
    lua_State* plua_state_;
    std::string error_str;
    LuaHeapRecycler lua_heap_recycler_;
private:
    DISALLOW_COPY_AND_ASSIGN(LuaState)
};

class LuaCmdLine : public CommonCmdLine
{
public:
    LuaCmdLine(const std::string& prompt = ">>>",
               const std::string& quit_cmd = "quit",
               bool is_case_sensitive = false);
    virtual ~LuaCmdLine();

    inline lua_State* getState() const { return lua_state_.getState(); }

    void registerFunction(const std::string& func_name, LuaCFunc lua_reg_func);
    virtual int parseFile(const std::string& file);
    virtual void process(const std::string& cmd);
    virtual void clear();
    void clearHeap() { lua_state_.clearHeap(); }
private:
    LuaState lua_state_;
private:
    DISALLOW_COPY_AND_ASSIGN(LuaCmdLine)
};

// C++ cal lua function
//int main(int argc, char* argv[])
//{
//    LuaState ls;
//    ls.parseFile("base.lua");
//    luaGetGlobal(ls.getState(), "add");
//    luaSetInteger(ls.getState(), 6);
//    luaSetInteger(ls.getState(), 9);
//    int err = luaCallFunc(ls.getState(), 2, 1);
//    if (err != 0)
//    {
//        printLine(luaGetError(ls.getState(), err));
//        return -1;
//    }
//
//    printLine("");
//    printLine("cnt = %d", luaGetTop(ls.getState()));
//    for(int i = 1; i<=luaGetTop(ls.getState()); ++i)
//    {
//        printLine(luaGetInteger(ls.getState(), i));
//    }
//    luaPop(ls.getState(), -1);
//}

} // namespace util

#endif // UTIL_LUAX_HPP
