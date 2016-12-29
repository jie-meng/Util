#include "lplatformext.hpp"
#include "util/luaextend.hpp"
#include "util/base.hpp"

namespace util
{

static int platformInfo(lua_State* plua_state)
{
    luaPushString(plua_state, util::platformInfo());
    return 1;
}

static const LuaReg platform_lib[] =
{
    {"platformInfo", platformInfo},

    {0, 0}
};

void extendPlatform(lua_State* plua_state) 
{
    LuaRegCombUtilLib::getInstance().addRegArray((LuaReg*)platform_lib);
}

}
