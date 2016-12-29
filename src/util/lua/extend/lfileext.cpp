#include "lfileext.hpp"
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
                                                luaGetInteger(plua_state, 2, 0),
                                                static_cast<char>(luaGetInteger(plua_state, 3, 0))
                                                ));

    return 1;
}

static int readBinaryFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, readBinaryFile(luaGetString(plua_state, 1, ""),
                                             static_cast<char*>(luaGetLightUserData(plua_state, 2, 0)),
                                             luaGetInteger(plua_state, 3, 0),
                                             static_cast<std::streamoff>(luaGetInteger(plua_state, 4, 0)),
                                             static_cast<std::ios::seekdir>(luaGetInteger(plua_state, 5, 0))
                                             ));
    return 1;
}

static int writeBinaryFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, writeBinaryFile(luaGetString(plua_state, 1, ""),
                                             static_cast<char*>(luaGetLightUserData(plua_state, 2, 0)),
                                             luaGetInteger(plua_state, 3, 0),
                                             luaGetBoolean(plua_state, 4, false)
                                             ));
    return 1;
}

static int overwriteBinaryFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, overwriteBinaryFile(luaGetString(plua_state, 1, ""),
                                             static_cast<char*>(luaGetLightUserData(plua_state, 2, 0)),
                                             luaGetInteger(plua_state, 3, 0),
                                             static_cast<std::streamoff>(luaGetInteger(plua_state, 4, 0)),
                                             static_cast<std::ios::seekdir>(luaGetInteger(plua_state, 5, 0))
                                             ));
    return 1;
}

//file info
static int fileSize(lua_State* plua_state)
{
    luaPushInteger(plua_state, fileSize(luaGetString(plua_state, 1, "")));
    return 1;
}

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

static int pathRename(lua_State* plua_state)
{
    luaPushBoolean(plua_state, pathRename(luaGetString(plua_state, 1, ""), luaGetString(plua_state, 2, "")));
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
static int appPath(lua_State* plua_state)
{
    luaPushString(plua_state, appPath());
    return 1;
}
    
static int currentPath(lua_State* plua_state)
{
    luaPushString(plua_state, currentPath());
    return 1;
}

static int setCurrentPath(lua_State* plua_state)
{
    luaPushBoolean(plua_state, setCurrentPath(luaGetString(plua_state, 1, "")));
    return 1;
}

static int relativePathToAbsolutePath(lua_State* plua_state)
{
    luaPushString(plua_state, relativePathToAbsolutePath(luaGetString(plua_state, 1, "")));
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
    std::vector<std::string> coll;
    findFilesInDir(luaGetString(plua_state, 1, ""), coll, luaGetString(plua_state, 2, ""));
    std::vector< std::pair<any, any> > vec;
    for (int i=0; i<(int)coll.size(); ++i)
        vec.push_back(std::make_pair(any(i+1), any(coll[i])));

    luaPushTable(plua_state, vec);
    return 1;
}

static int findPathInDir(lua_State* plua_state)
{
    std::vector<std::string> coll;
    findPathInDir(luaGetString(plua_state, 1, ""), coll);
    std::vector< std::pair<any, any> > vec;
    for (int i=0; i<(int)coll.size(); ++i)
        vec.push_back(std::make_pair(any(i+1), any(coll[i])));

    luaPushTable(plua_state, vec);
    return 1;
}

static int findFilesInDirRecursively(lua_State* plua_state)
{
    std::vector<std::string> coll;
    findFilesInDirRecursively(luaGetString(plua_state, 1, ""), coll, luaGetString(plua_state, 2, ""));
    std::vector< std::pair<any, any> > vec;
    for (int i=0; i<(int)coll.size(); ++i)
        vec.push_back(std::make_pair(any(i+1), any(coll[i])));

    luaPushTable(plua_state, vec);
    return 1;
}

static const LuaReg file_lib[] =
{
    {"readTextFile", readTextFile},
    {"writeTextFile", writeTextFile},
    {"createBinaryFile", createBinaryFile},
    {"readBinaryFile", readBinaryFile},
    {"writeBinaryFile", writeBinaryFile},
    {"overwriteBinaryFile", overwriteBinaryFile},

    {"fileSize", fileSize},
    {"fileExtension", fileExtension},
    {"fileBaseName", fileBaseName},
    {"fileCopy", fileCopy},

    {"appPath", appPath},
    {"currentPath", currentPath},
    {"setCurrentPath", setCurrentPath},
    {"relativePathToAbsolutePath", relativePathToAbsolutePath},
    {"isPathExists", isPathExists},
    {"isPathDir", isPathDir},
    {"isPathFile", isPathFile},
    {"isPathEmpty", isPathEmpty},
    {"pathRemoveAll", pathRemoveAll},
    {"pathRemove", pathRemove},
    {"pathRename", pathRename},
    {"splitPathname", splitPathname},
    {"mkDir", mkDir},

    {"findFilesInDir", findFilesInDir},
    {"findPathInDir", findPathInDir},
    {"findFilesInDirRecursively", findFilesInDirRecursively},

    {0, 0}
};

void extendFile(lua_State* plua_state) 
{
    LuaRegCombUtilLib::getInstance().addRegArray((LuaReg*)file_lib);
}

}
