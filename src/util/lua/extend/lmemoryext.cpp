#include "lmemoryext.hpp"
#include "util/luaextend.hpp"

namespace util
{

const std::string kByteMemoryHandle = "Memory*";

typedef Memory<char> ByteMemory;

static int createMemory(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "create",
        luaGetTop(plua_state) > 0 && LuaNumber == luaGetType(plua_state, 1), "create memory with incorrect size");

    size_t memory_size = luaGetInteger(plua_state, 1);    
    LuaObject<ByteMemory>* p = luaNewEmptyObject<ByteMemory>(plua_state, kByteMemoryHandle);
    p->setData(new ByteMemory(memory_size));

    return 1;
}

static int memoryDestroy(lua_State* plua_state)
{
    return luaObjectDestroy<ByteMemory>(plua_state, kByteMemoryHandle);
}

static int memoryToString(lua_State* plua_state)
{
    return luaObjectToString<ByteMemory>(plua_state, kByteMemoryHandle);
}

static int memorySize(lua_State* plua_state)
{    
    ByteMemory* pmemory = luaGetObjectData<ByteMemory>(plua_state, kByteMemoryHandle);
    luaPushInteger(plua_state, pmemory->size());

    return 1;
}

static int memoryBuf(lua_State* plua_state)
{
    ByteMemory* pmemory = luaGetObjectData<ByteMemory>(plua_state, kByteMemoryHandle);
    luaPushLightUserData(plua_state, pmemory->buf());

    return 1;
}

static int memoryClear(lua_State* plua_state)
{
    ByteMemory* pmemory = luaGetObjectData<ByteMemory>(plua_state, kByteMemoryHandle);
    pmemory->clear();

    return 0;
}

static int memOffset(lua_State* plua_state)
{
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memOffset", p, "address is null");

    luaPushLightUserData(plua_state, (void*)(p + luaGetInteger(plua_state, 2, 0)));

    return 1;
}

static int mem_content(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memContent", luaGetTop(plua_state) == 2,
        "parameter count should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memContent", p, "address is null");

    luaPushString(plua_state, memContent(p, luaGetInteger(plua_state, 2)));

    return 1;
}

static int memToString(lua_State* plua_state)
{
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memToString", p, "c-string address is null");

    luaPushString(plua_state, toString(p));

    return 1;
}

static int memset(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memset", 3 == luaGetTop(plua_state),
        "parameter count should be 3");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memset", p, "address is null");

    ::memset(p, luaGetInteger(plua_state, 2), luaGetInteger(plua_state, 3));

    return 0;
}

static int memcpy(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memcpy", 3 == luaGetTop(plua_state),
        "parameter count should be 3");
    char* p1 = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memcpy", p1,
        "dest address is null");

    char* p2 = static_cast<char*>(luaGetLightUserData(plua_state, 2));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memcpy", p2,
        "src address is null");

    ::memcpy(p1, p2, luaGetInteger(plua_state, 3, 0));

    return 0;
}

static int memmove(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memmove", 3 == luaGetTop(plua_state),
        "parameter count should be 3");
    char* p1 = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memmove", p1,
        "dest address is null");

    char* p2 = static_cast<char*>(luaGetLightUserData(plua_state, 2));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memmove", p2,
        "src address is null");

    ::memmove(p1, p2, luaGetInteger(plua_state, 3, 0));

    return 0;
}

static int strcpy(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "strcpy", 2 == luaGetTop(plua_state),
        "parameter count should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "strcpy", p,
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
static int memSetUInt(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memSetUnit", 2 == luaGetTop(plua_state),
        "parameter count should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memSetUnit", p,
        "address is null");

    *(T*)p = luaGetInteger(plua_state, 2);

    return 0;
}

template <typename T>
static int memGetUInt(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memGetUnit", 1 == luaGetTop(plua_state),
        "parameter count should be 1");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memGetUnit", p,
        "address is null");

    luaPushInteger(plua_state, *(T*)p);

    return 1;
}

static int memSetBytes(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memSetBytes", luaGetTop(plua_state) > 1,
        "parameter count should not less than 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memSetBytes", p,
        "address is null");

    for (size_t i=2; i<=(size_t)luaGetTop(plua_state); ++i)
    {
        luaExtendAssert(plua_state, kLuaExtendLibUtil, "memSetBytes", luaGetInteger(plua_state, i) <= 255,
            "byte value should not be more than 255");
        char* p1 = p + (i-2);
        *(uint8_t*)(p1) = luaGetInteger(plua_state, i);
    }

    return 0;
}

static const LuaReg memory_lib[] =
{
    {"newMemory", createMemory},
        
    {"memset", memset},
    {"memcpy", memcpy},
    {"memmove", memmove},
    {"strcpy", strcpy},

    {"memOffset", memOffset},
    {"memContent", mem_content},
    {"memToString", memToString},
    {"memSetUInt8", memSetUInt<uint8_t>},
    {"memSetUInt16", memSetUInt<uint16_t>},
    {"memSetUInt32", memSetUInt<uint32_t>},
    {"memGetUInt8", memGetUInt<uint8_t>},
    {"memGetUInt16", memGetUInt<uint16_t>},
    {"memGetUInt32", memGetUInt<uint32_t>},
    {"memSetBytes", memSetBytes},

    {0, 0}
};

static const LuaReg memory_obj_lib[] =
{
    {"delete", memoryDestroy},
    {"size", memorySize},
    {"clear", memoryClear},
    {"buf", memoryBuf},
    {"__gc", memoryDestroy},
    {"__tostring", memoryToString},
    {0, 0}
};

void extendMemory(lua_State* plua_state) 
{
    LuaRegCombUtilLib::getInstance().addRegArray((LuaReg*)memory_lib);
    luaCreateMeta(plua_state, kByteMemoryHandle, (LuaReg*)memory_obj_lib);
}

}
