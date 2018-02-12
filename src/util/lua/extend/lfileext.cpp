#include "lfileext.hpp"
#include "util/luaextend.hpp"
#include "util/file.hpp"

namespace util
{

//file read & write
static int readTextFile(lua_State* plua_state)
{
    luaPushString(plua_state, readTextFile(luaToString(plua_state, 1, "")));
    return 1;
}

static int writeTextFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, writeTextFile(luaToString(plua_state, 1, ""),
                                             luaToString(plua_state, 2, ""),
                                             luaToBoolean(plua_state, 3, false)
                                             ));
    return 1;
}

static int createBinaryFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, createBinaryFile(luaToString(plua_state, 1, ""),
                                                luaToInteger(plua_state, 2, 0),
                                                static_cast<char>(luaToInteger(plua_state, 3, 0))
                                                ));

    return 1;
}

static int readBinaryFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, readBinaryFile(luaToString(plua_state, 1, ""),
                                             static_cast<char*>(luaToLightUserData(plua_state, 2, 0)),
                                             luaToInteger(plua_state, 3, 0),
                                             static_cast<std::streamoff>(luaToInteger(plua_state, 4, 0)),
                                             static_cast<std::ios::seekdir>(luaToInteger(plua_state, 5, 0))
                                             ));
    return 1;
}

static int writeBinaryFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, writeBinaryFile(luaToString(plua_state, 1, ""),
                                             static_cast<char*>(luaToLightUserData(plua_state, 2, 0)),
                                             luaToInteger(plua_state, 3, 0),
                                             luaToBoolean(plua_state, 4, false)
                                             ));
    return 1;
}

static int overwriteBinaryFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, overwriteBinaryFile(luaToString(plua_state, 1, ""),
                                             static_cast<char*>(luaToLightUserData(plua_state, 2, 0)),
                                             luaToInteger(plua_state, 3, 0),
                                             static_cast<std::streamoff>(luaToInteger(plua_state, 4, 0)),
                                             static_cast<std::ios::seekdir>(luaToInteger(plua_state, 5, 0))
                                             ));
    return 1;
}

//file info
static int fileSize(lua_State* plua_state)
{
    luaPushInteger(plua_state, fileSize(luaToString(plua_state, 1, "")));
    return 1;
}

static int fileExtension(lua_State* plua_state)
{
    luaPushString(plua_state, fileExtension(luaToString(plua_state, 1, "")));
    return 1;
}

static int fileBaseName(lua_State* plua_state)
{
    luaPushString(plua_state, fileBaseName(luaToString(plua_state, 1, "")));
    return 1;
}

static int isTextFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, isTextFile(luaToString(plua_state, 1, "")));
    return 1;
}

static int pathRename(lua_State* plua_state)
{
    luaPushBoolean(plua_state, pathRename(luaToString(plua_state, 1, ""), luaToString(plua_state, 2, "")));
    return 1;
}

static int fileCopy(lua_State* plua_state)
{
    luaPushBoolean(plua_state, fileCopy(luaToString(plua_state, 1, ""),
                                          luaToString(plua_state, 2, ""),
                                          luaToBoolean(plua_state, 3, true)
                                          ));
    return 1;
}

static int fileCopyFullPath(lua_State* plua_state)
{
    luaPushBoolean(plua_state, fileCopyFullPath(luaToString(plua_state, 1, ""),
                                                  luaToString(plua_state, 2, ""),
                                                  luaToBoolean(plua_state, 3, true)
                                                  ));
    return 1;
}

static int copyTree(lua_State* plua_state)
{
    copyTree(luaToString(plua_state, 1, ""),
              luaToString(plua_state, 2, ""),
              luaToBoolean(plua_state, 3, true));
    return 0;
}

static int splitPathname(lua_State* plua_state)
{
    std::pair<std::string, std::string> ret = splitPathname(luaToString(plua_state, 1, ""));
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
    luaPushBoolean(plua_state, setCurrentPath(luaToString(plua_state, 1, "")));
    return 1;
}

static int relativePathToAbsolutePath(lua_State* plua_state)
{
    luaPushString(plua_state, relativePathToAbsolutePath(luaToString(plua_state, 1, "")));
    return 1;
}

static int isPathExists(lua_State* plua_state)
{
    luaPushBoolean(plua_state, isPathExists(luaToString(plua_state, 1, "")));
    return 1;
}

static int isPathDir(lua_State* plua_state)
{
    luaPushBoolean(plua_state, isPathDir(luaToString(plua_state, 1, "")));
    return 1;
}

static int isPathFile(lua_State* plua_state)
{
    luaPushBoolean(plua_state, isPathFile(luaToString(plua_state, 1, "")));
    return 1;
}

static int isPathEmpty(lua_State* plua_state)
{
    luaPushBoolean(plua_state, isPathEmpty(luaToString(plua_state, 1, "")));
    return 1;
}

static int pathRemoveAll(lua_State* plua_state)
{
    pathRemoveAll(luaToString(plua_state, 1, ""));
    return 0;
}

static int pathRemove(lua_State* plua_state)
{
    luaPushBoolean(plua_state, pathRemove(luaToString(plua_state, 1, "")));
    return 1;
}

static int mkDir(lua_State* plua_state)
{
    luaPushBoolean(plua_state, mkDir(luaToString(plua_state, 1, ""), luaToBoolean(plua_state, 1, true)));
    return 1;
}

static int mkFullDir(lua_State* plua_state)
{
    luaPushBoolean(plua_state, mkFullDir(luaToString(plua_state, 1, "")));
    return 1;
}


//dir
static int findFilesInDir(lua_State* plua_state)
{
    std::vector<std::string> coll;
    findFilesInDir(luaToString(plua_state, 1, ""), coll, luaToString(plua_state, 2, ""));
    std::vector< std::pair<any, any> > vec;
    for (int i=0; i<(int)coll.size(); ++i)
        vec.push_back(std::make_pair(any(i+1), any(coll[i])));

    luaPushArray(plua_state, vec);
    return 1;
}

static int findPathInDir(lua_State* plua_state)
{
    std::vector<std::string> coll;
    findPathInDir(luaToString(plua_state, 1, ""), coll);
    std::vector< std::pair<any, any> > vec;
    for (int i=0; i<(int)coll.size(); ++i)
        vec.push_back(std::make_pair(any(i+1), any(coll[i])));

    luaPushArray(plua_state, vec);
    return 1;
}

static int findFilesInDirRecursively(lua_State* plua_state)
{
    std::vector<std::string> coll;
    findFilesInDirRecursively(luaToString(plua_state, 1, ""), coll, luaToString(plua_state, 2, ""));
    std::vector< std::pair<any, any> > vec;
    for (int i=0; i<(int)coll.size(); ++i)
        vec.push_back(std::make_pair(any(i+1), any(coll[i])));

    luaPushArray(plua_state, vec);
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
    {"fileCopy", fileCopy},
    {"fileCopyFullPath", fileCopyFullPath},
    {"copyTree", copyTree},
        
    {"fileSize", fileSize},
    {"fileExtension", fileExtension},
    {"fileBaseName", fileBaseName},
    {"isTextFile", isTextFile},

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
    {"mkFullDir", mkFullDir},

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
