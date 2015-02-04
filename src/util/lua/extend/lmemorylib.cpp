#include "lmemorylib.hpp"
#include "util/luaextend.hpp"

namespace util
{

typedef Memory<char> ByteMemory;

static int create(lua_State* plua_state)
{
    luaAssert(plua_state, luaGetTop(plua_state) > 0 &&
              LuaNumber == luaGetType(plua_state, 1), "create memory with incorrect size");

    size_t memory_size = luaGetInteger(plua_state, 1);

    ByteMemory* pmemory = new ByteMemory(memory_size);

    LuaHeapRecyclerManager::getInstance().addHeapObject(plua_state, (void*)pmemory, deleteVoid<ByteMemory>);

    luaPushLightUserData(plua_state, (void*)pmemory);

    return 1;
}

static int destroy(lua_State* plua_state)
{
    ByteMemory* pmemory = static_cast<ByteMemory*>(luaGetLightUserData(plua_state, 1, 0));

    LuaHeapRecyclerManager::getInstance().removeHeapObject(plua_state, (void*)pmemory);

    if (pmemory)
        delete pmemory;

    return 0;
}

static int size(lua_State* plua_state)
{
    ByteMemory* pmemory = static_cast<ByteMemory*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pmemory)
        luaPushInteger(plua_state, 0);
    else
        luaPushInteger(plua_state, pmemory->size());

    return 1;
}

static int buf(lua_State* plua_state)
{
    ByteMemory* pmemory = static_cast<ByteMemory*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pmemory)
        luaPushNil(plua_state);
    else
        luaPushLightUserData(plua_state, pmemory->buf());

    return 1;
}

static int clear(lua_State* plua_state)
{
    ByteMemory* pmemory = static_cast<ByteMemory*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pmemory)
        return 0;

    pmemory->clear();

    return 0;
}

static int offset(lua_State* plua_state)
{
    luaAssert(plua_state, 2 == luaGetTop(plua_state), "[offset] param cnt should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    if (!p)
    {
        luaPushLightUserData(plua_state, NULL);
        return 1;
    }

    luaPushLightUserData(plua_state, (void*)(p + luaGetInteger(plua_state, 2, 0)));

    return 1;
}

static int memstr(lua_State* plua_state)
{
    luaAssert(plua_state, 2 == luaGetTop(plua_state), "[memstr] param cnt should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    if (!p)
    {
        luaPushString(plua_state, "");
        return 1;
    }

    luaPushString(plua_state, memoryStr(p, luaGetInteger(plua_state, 2)));

    return 1;
}

static int tostring(lua_State* plua_state)
{
    luaAssert(plua_state, 1 == luaGetTop(plua_state), "[tostring] param cnt should be 1");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    if (!p)
    {
        luaPushString(plua_state, "");
        return 1;
    }

    luaPushString(plua_state, toString(p));

    return 1;
}

static int memset(lua_State* plua_state)
{
    luaAssert(plua_state, 3 == luaGetTop(plua_state), "[memset] param cnt should be 3");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    if(!p)
        return 0;

    ::memset(p, luaGetInteger(plua_state, 2), luaGetInteger(plua_state, 3));

    return 0;
}

static int memcpy(lua_State* plua_state)
{
    luaAssert(plua_state, 3 == luaGetTop(plua_state), "[memcpy] param cnt should be 3");
    char* p1 = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    if (!p1)
        return 0;

    char* p2 = static_cast<char*>(luaGetLightUserData(plua_state, 2));
    if (!p2)
        return 0;

    ::memcpy(p1, p2, luaGetInteger(plua_state, 3));

    return 0;
}

static int memmove(lua_State* plua_state)
{
    luaAssert(plua_state, 3 == luaGetTop(plua_state), "[memmove] param cnt should be 3");
    char* p1 = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    if (!p1)
        return 0;

    char* p2 = static_cast<char*>(luaGetLightUserData(plua_state, 2));
    if (!p2)
        return 0;

    ::memmove(p1, p2, luaGetInteger(plua_state, 3));

    return 0;
}

static int strcpy(lua_State* plua_state)
{
    luaAssert(plua_state, 2 == luaGetTop(plua_state), "[strcpy] param cnt should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    if (!p)
        return 0;

    if(LuaLightUserData == luaGetType(plua_state, 2))
    {
        char* p2 = static_cast<char*>(luaGetLightUserData(plua_state, 2));
        if(p2)
            ::strcpy(p, p2);
    }
    else if (LuaString == luaGetType(plua_state, 2))
    {
        std::string str = luaGetString(plua_state, 2);
        ::strcpy(p, str.c_str());
    }

    return 0;
}

template <typename T>
static int setUint(lua_State* plua_state)
{
    luaAssert(plua_state, 2 == luaGetTop(plua_state), "[setUint] param cnt should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    if (!p)
        return 0;

    *(T*)p = luaGetInteger(plua_state, 2);

    return 0;
}

template <typename T>
static int getUint(lua_State* plua_state)
{
    luaAssert(plua_state, 1 == luaGetTop(plua_state), "[getUint] param cnt should be 1");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaAssert(plua_state, NULL != p, "[getUint] memory address is NULL");

    luaPushInteger(plua_state, *(T*)p);

    return 1;
}

static int setBytes(lua_State* plua_state)
{
    luaAssert(plua_state, luaGetTop(plua_state) > 1, "[setMemBytes] param cnt should not less than 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    if (!p)
        return 0;

    for (size_t i=2; i<=(size_t)luaGetTop(plua_state); ++i)
    {
        char* p1 = p + (i-2);
        luaAssert(plua_state, luaGetInteger(plua_state, i) <= 255, "[setMemBytes] byte value should not be more than 255");
        *(uint8_t*)(p1) = luaGetInteger(plua_state, i);
    }

    return 0;
}

static const u_luaL_Reg memory_lib[] =
{
    {"create", create},
    {"destroy", destroy},
    {"size", size},
    {"clear", clear},
    {"buf", buf},

    {"offset", offset},
    {"memset", memset},
    {"memcpy", memcpy},
    {"memmove", memmove},
    {"strcpy", strcpy},
    {"memstr", memstr},
    {"tostring", tostring},

    {"setUint8", setUint<uint8_t>},
    {"setUint16", setUint<uint16_t>},
    {"setUint32", setUint<uint32_t>},
    {"getUint8", getUint<uint8_t>},
    {"getUint16", getUint<uint16_t>},
    {"getUint32", getUint<uint32_t>},
    {"setBytes", setBytes},

    {0, 0}
};

/*
** Open regex library
*/
int lualibMemoryCreate(lua_State* plua_state) {

    luaCreateLib(plua_state, (u_luaL_Reg*)memory_lib);
    return 1;
}

}
