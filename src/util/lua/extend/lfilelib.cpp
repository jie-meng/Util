#include "lfilelib.hpp"
#include "util/luaextend.hpp"
#include "util/file.hpp"

namespace util
{

//file read & write
static int readTextFile(lua_State* plua_state)
{
    luaPushString(plua_state, readTextFile(luaGetString(plua_state, 1, "")));
    return 1;
}

static int writeTextFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, writeTextFile(luaGetString(plua_state, 1, ""),
                                             luaGetString(plua_state, 2, ""),
                                             luaGetBoolean(plua_state, 3, false)
                                             ));
    return 1;
}

static int createBinaryFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, createBinaryFile(luaGetString(plua_state, 1, ""),
                                                luaGetUInteger(plua_state, 2, 0),
                                                static_cast<char>(luaGetInteger(plua_state, 3, 0))
                                                ));

    return 1;
}

static int readBinaryFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, readBinaryFile(luaGetString(plua_state, 1, ""),
                                             static_cast<char*>(luaGetLightUserData(plua_state, 2, 0)),
                                             luaGetUInteger(plua_state, 3, 0),
                                             static_cast<std::streamoff>(luaGetUInteger(plua_state, 4, 0)),
                                             static_cast<std::ios::seekdir>(luaGetUInteger(plua_state, 5, 0))
                                             ));
    return 1;
}

static int writeBinaryFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, writeBinaryFile(luaGetString(plua_state, 1, ""),
                                             static_cast<char*>(luaGetLightUserData(plua_state, 2, 0)),
                                             luaGetUInteger(plua_state, 3, 0),
                                             luaGetBoolean(plua_state, 4, false)
                                             ));
    return 1;
}

static int overwriteBinaryFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, overwriteBinaryFile(luaGetString(plua_state, 1, ""),
                                             static_cast<char*>(luaGetLightUserData(plua_state, 2, 0)),
                                             luaGetUInteger(plua_state, 3, 0),
                                             static_cast<std::streamoff>(luaGetUInteger(plua_state, 4, 0)),
                                             static_cast<std::ios::seekdir>(luaGetUInteger(plua_state, 5, 0))
                                             ));
    return 1;
}

//file info
static int fileExtension(lua_State* plua_state)
{
    luaPushString(plua_state, fileExtension(luaGetString(plua_state, 1, "")));
    return 1;
}

static int fileBaseName(lua_State* plua_state)
{
    luaPushString(plua_state, fileBaseName(luaGetString(plua_state, 1, "")));
    return 1;
}

static int fileRename(lua_State* plua_state)
{
    luaPushBoolean(plua_state, fileRename(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, "")));
    return 1;
}

static int fileCopy(lua_State* plua_state)
{
    luaPushBoolean(plua_state, fileCopy(luaGetString(plua_state, 1, ""),
                                          luaGetString(plua_state, 2, ""),
                                          luaGetBoolean(plua_state, 3, true)
                                          ));
    return 1;
}

static int splitPathname(lua_State* plua_state)
{
    std::pair<std::string, std::string> ret = splitPathname(luaGetString(plua_state, 1, ""));
    luaPushString(plua_state, ret.first);
    luaPushString(plua_state, ret.second);

    return 2;
}

//path
static int currentPath(lua_State* plua_state)
{
    luaPushString(plua_state, currentPath());
    return 1;
}

static int isPathExists(lua_State* plua_state)
{
    luaPushBoolean(plua_state, isPathExists(luaGetString(plua_state, 1, "")));
    return 1;
}

static int isPathDir(lua_State* plua_state)
{
    luaPushBoolean(plua_state, isPathDir(luaGetString(plua_state, 1, "")));
    return 1;
}

static int isPathFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, isPathFile(luaGetString(plua_state, 1, "")));
    return 1;
}

static int isPathEmpty(lua_State* plua_state)
{
    luaPushBoolean(plua_state, isPathEmpty(luaGetString(plua_state, 1, "")));
    return 1;
}

static int pathRemoveAll(lua_State* plua_state)
{
    pathRemoveAll(luaGetString(plua_state, 1, ""));
    return 0;
}

static int pathRemove(lua_State* plua_state)
{
    luaPushBoolean(plua_state, pathRemove(luaGetString(plua_state, 1, "")));
    return 1;
}

static int mkDir(lua_State* plua_state)
{
    luaPushBoolean(plua_state, mkDir(luaGetString(plua_state, 1, "")));
    return 1;
}

//dir
static int findFilesInDir(lua_State* plua_state)
{
    std::list<std::string> coll;
    findFilesInDir(luaGetString(plua_state, 1, ""), coll, luaGetString(plua_state, 2, ""));
    luaPushString(plua_state, strJoin(coll.begin(), coll.end(), luaGetString(plua_state, 3, "\n")));
    return 1;
}

static int findPathInDir(lua_State* plua_state)
{
    std::list<std::string> coll;
    findPathInDir(luaGetString(plua_state, 1, ""), coll);
    luaPushString(plua_state, strJoin(coll.begin(), coll.end(), luaGetString(plua_state, 2, "\n")));
    return 1;
}

static int findFilesInDirRecursively(lua_State* plua_state)
{
    std::list<std::string> coll;
    findFilesInDirRecursively(luaGetString(plua_state, 1, ""), coll, luaGetString(plua_state, 2, ""));
    luaPushString(plua_state, strJoin(coll.begin(), coll.end(), luaGetString(plua_state, 3, "\n")));
    return 1;
}

static const u_luaL_Reg file_lib[] =
{
    {"readTextFile", readTextFile},
    {"writeTextFile", writeTextFile},
    {"createBinaryFile", createBinaryFile},
    {"readBinaryFile", readBinaryFile},
    {"writeBinaryFile", writeBinaryFile},
    {"overwriteBinaryFile", overwriteBinaryFile},

    {"fileExtension", fileExtension},
    {"fileBaseName", fileBaseName},
    {"fileRename", fileRename},
    {"fileCopy", fileCopy},
    {"splitPathname", splitPathname},

    {"currentPath", currentPath},
    {"isPathExists", isPathExists},
    {"isPathDir", isPathDir},
    {"isPathFile", isPathFile},
    {"isPathEmpty", isPathEmpty},
    {"pathRemoveAll", pathRemoveAll},
    {"pathRemove", pathRemove},
    {"mkDir", mkDir},

    {"findFilesInDir", findFilesInDir},
    {"findPathInDir", findPathInDir},
    {"findFilesInDirRecursively", findFilesInDirRecursively},

    {0, 0}
};

/*
** Open regex library
*/
int lualibFileCreate(lua_State* plua_state) {

    luaCreateLib(plua_state, (u_luaL_Reg*)file_lib);
    return 1;
}

}
