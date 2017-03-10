#include "base.hpp"
#include <map>
#include <set>
#include "luaextend.hpp"
#include "thread.hpp"
#include "lua/src/lua.hpp"

namespace util
{
    
std::string toString(LuaType lua_type)
{
    switch (lua_type)
    {
        case LuaNone:
            return "LuaNone";
        case LuaNil:
            return "LuaNil";
        case LuaBoolean:
            return "LuaBoolean";
        case LuaLightUserData:
            return "LuaLightUserData";
        case LuaNumber:
            return "LuaNumber";
        case LuaString:
            return "LuaString";
        case LuaTable:
            return "LuaTable";
        case LuaFunction:
            return "LuaFunction";
        case LuaUserData:
            return "LuaUserData";
        case LuaThread:
            return "LuaThread";
        case LuaNumTags:
            return "LuaNumTags";
        default:
            throw Exception(strFormat("Invalid LuaType value: %d", (int)lua_type));
    }
}

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

std::string luaGetString(lua_State* plua_state, int index)
{
    //return std::string(luaL_checkstring(plua_state, index));
    const char* result = lua_tostring(plua_state, index);
    if (!result)
        luaError(plua_state, strFormat("luaGetString from index %d failed.", index));

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

LuaCFunc luaGetFunction(lua_State* plua_state, int index)
{
    return (LuaCFunc)lua_tocfunction(plua_state, index);
}

LuaCFunc luaGetFunction(lua_State* plua_state, int index, LuaCFunc default_function)
{
    return lua_isfunction(plua_state, index) ? luaGetFunction(plua_state, index) : default_function;
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

std::vector< std::pair<any, any> > luaGetTable(lua_State* plua_state, int index)
{
    std::vector< std::pair<any, any> > vec;

    if (!lua_istable(plua_state, index))
        return vec;

    // Process of lua_next:
    // 1) Pop a key from stack
    // 2) Get a key-value pair from specified position, push key and then value to stack
    // 3) If 2) success then return non-zero, else return zero, and do not push any thing to stack.
    // The key-value get from table is related to the popped key in 1).
    // There is nothing before first pair of key-value, so we use lua_pushnil() to push a nil as first key.
    lua_pushnil(plua_state);
    while(0 != lua_next(plua_state, index))
    {
        //Now value is at (-1), key is at (-2)
        vec.push_back(std::make_pair(luaGetAny(plua_state, -2), luaGetAny(plua_state, -1)));
        //remove value from stack, make the key to the top in order to keep on enumerating
        lua_pop(plua_state, 1);
    }

    return vec;
}

//push operate
void luaPushDouble(lua_State* plua_state, double double_val)
{
    lua_pushnumber(plua_state, double_val);
}

void luaPushInteger(lua_State* plua_state, int int_val)
{
    lua_pushinteger(plua_state, int_val);
}

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
    else if (a.type() == typeid(size_t))
    {
        luaPushInteger(plua_state, (int)any_cast<size_t>(a));
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

void luaPushTable(lua_State* plua_state, const std::vector<any>& vec)
{
    lua_newtable(plua_state);

    for (size_t i=0; i<vec.size(); ++i)
    {
        luaPushInteger(plua_state, i+1);
        luaPushAny(plua_state, vec[i]);
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

void luaPop(lua_State* plua_state, int count)
{
    lua_pop(plua_state, count);
}

int luaGetTop(lua_State* plua_state)
{
    return lua_gettop(plua_state);
}

void luaGetGlobal(lua_State* plua_state, const std::string& name)
{
    lua_getglobal(plua_state, name.c_str());
}

void luaSetGlobal(lua_State* plua_state, const std::string& name)
{
    lua_setglobal(plua_state, name.c_str());
}

int luaCallFunc(lua_State* plua_state, int nargs, int nrets)
{
    return lua_pcall(plua_state, nargs, nrets, 0);
}

void luaAssert(lua_State* plua_state, bool assertion, const std::string& str)
{
    if (!plua_state)
        luaL_error(plua_state, "plua_state is null");

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
            err_type = "syntax error during pre-compilation";
            break;
        case LUA_ERRMEM: //memory error
            err_type = "memory allocation error";
            break;
        case LUA_ERRRUN: //runtime-time error
            err_type = "runtime error";
            break;
        case LUA_YIELD: //thread suspend error
            err_type = "thread has been suspended";
            break;
        case LUA_ERRERR: //error while running
            err_type = "error while running the error handler function";
            break;
        default:
            err_type = "unknown";
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

void* luaNewUserData(lua_State* plua_state, size_t sz)
{
    return lua_newuserdata(plua_state, sz);
}

void luaSetMetaTable(lua_State* plua_state, const std::string& tname)
{
    luaL_setmetatable(plua_state, tname.c_str());
}

void* luaCheckUData(lua_State* plua_state, int ud, const std::string& tname)
{
    return luaL_checkudata(plua_state, ud, tname.c_str());
}

int luaFileresult(lua_State* plua_state, bool stat, const std::string& fname)
{
    return fname.empty() ?
        luaL_fileresult(plua_state, stat, NULL) :
        luaL_fileresult(plua_state, stat, fname.c_str());
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

    return true;
}

void LuaState::cleanup()
{
    if (plua_state_)
        lua_close(plua_state_);

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

int LuaState::parseLine(const std::string& line)
{
    return luaParseLine(plua_state_, line, error_str);
}

int LuaState::parseFile(const std::string& file)
{
    return luaParseFile(plua_state_, file, error_str);
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
    if(strAreEqual(cmd, "clearheap", isCaseSensitive()))
    {
        return;
    }
    else if(strAreEqual(cmd, "clear", isCaseSensitive()))
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
