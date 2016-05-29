#include "lmemorylib.hpp"
#include "util/luaextend.hpp"

namespace util
{

typedef Memory<char> ByteMemory;

static int create(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "create",
        luaGetTop(plua_state) > 0 && LuaNumber == luaGetType(plua_state, 1), "create memory with incorrect size");

    size_t memory_size = luaGetInteger(plua_state, 1);
    ByteMemory* pmemory = new ByteMemory(memory_size);
    LuaHeapRecyclerManager::getInstance().addHeapObject<ByteMemory>(plua_state, (void*)pmemory);

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
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "size", pmemory, "null pointer");

    luaPushInteger(plua_state, pmemory->size());

    return 1;
}

static int buf(lua_State* plua_state)
{
    ByteMemory* pmemory = static_cast<ByteMemory*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "buf", pmemory, "null pointer");

    luaPushLightUserData(plua_state, pmemory->buf());

    return 1;
}

static int clear(lua_State* plua_state)
{
    ByteMemory* pmemory = static_cast<ByteMemory*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "clear", pmemory, "null pointer");

    pmemory->clear();

    return 0;
}

static int offset(lua_State* plua_state)
{
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "offset", p, "address is null");

    luaPushLightUserData(plua_state, (void*)(p + luaGetInteger(plua_state, 2, 0)));

    return 1;
}

static int memstr(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "memstr", luaGetTop(plua_state) == 2,
        "parameter count should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "memstr", p, "address is null");

    luaPushString(plua_state, memoryStr(p, luaGetInteger(plua_state, 2)));

    return 1;
}

static int tostring(lua_State* plua_state)
{
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "tostring", p, "c-string address is null");

    luaPushString(plua_state, toString(p));

    return 1;
}

static int memset(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "memset", 3 == luaGetTop(plua_state),
        "parameter count should be 3");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "memset", p, "address is null");

    ::memset(p, luaGetInteger(plua_state, 2), luaGetInteger(plua_state, 3));

    return 0;
}

static int memcpy(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "memcpy", 3 == luaGetTop(plua_state),
        "parameter count should be 3");
    char* p1 = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "memcpy", p1,
        "dest address is null");

    char* p2 = static_cast<char*>(luaGetLightUserData(plua_state, 2));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "memcpy", p2,
        "src address is null");

    ::memcpy(p1, p2, luaGetInteger(plua_state, 3, 0));

    return 0;
}

static int memmove(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "memmove", 3 == luaGetTop(plua_state),
        "parameter count should be 3");
    char* p1 = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "memmove", p1,
        "dest address is null");

    char* p2 = static_cast<char*>(luaGetLightUserData(plua_state, 2));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "memmove", p2,
        "src address is null");

    ::memmove(p1, p2, luaGetInteger(plua_state, 3, 0));

    return 0;
}

static int strcpy(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "strcpy", 2 == luaGetTop(plua_state),
        "parameter count should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "strcpy", p,
        "dest address is null");

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
static int setUInt(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "setUnit", 2 == luaGetTop(plua_state),
        "parameter count should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "setUnit", p,
        "address is null");

    *(T*)p = luaGetInteger(plua_state, 2);

    return 0;
}

template <typename T>
static int getUInt(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "getUnit", 1 == luaGetTop(plua_state),
        "parameter count should be 1");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "getUnit", p,
        "address is null");

    luaPushInteger(plua_state, *(T*)p);

    return 1;
}

static int setBytes(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "setBytes", luaGetTop(plua_state) > 1,
        "parameter count should not less than 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMemory, "setBytes", p,
        "address is null");

    for (size_t i=2; i<=(size_t)luaGetTop(plua_state); ++i)
    {
        luaExtendAssert(plua_state, kLuaExtendLibMemory, "setBytes", luaGetInteger(plua_state, i) <= 255,
            "byte value should not be more than 255");
        char* p1 = p + (i-2);
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

    {"setUInt8", setUInt<uint8_t>},
    {"setUInt16", setUInt<uint16_t>},
    {"setUInt32", setUInt<uint32_t>},
    {"getUInt8", getUInt<uint8_t>},
    {"getUInt16", getUInt<uint16_t>},
    {"getUInt32", getUInt<uint32_t>},
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
