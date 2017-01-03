#include "lutillib.hpp"
#include "util/luaextend.hpp"
#include "lplatformext.hpp"
#include "lstringext.hpp"
#include "lmemoryext.hpp"
#include "lregexext.hpp"
#include "lnetext.hpp"
#include "lfileext.hpp"
#include "lcsvext.hpp"
#include "lcfgext.hpp"
#include "lcmdlineext.hpp"
#include "lthreadext.hpp"

namespace util
{

int lualibUtilCreate(lua_State* plua_state)
{
    extendPlatform(plua_state);
    extendString(plua_state);
    extendCfg(plua_state);
    extendCsv(plua_state);
    extendFile(plua_state);
    extendMemory(plua_state);
    extendRegex(plua_state);
    extendCmdLine(plua_state);
    extendNet(plua_state);
    extendThread(plua_state);
    
    luaCreateLib(plua_state, LuaRegCombUtilLib::getInstance().getRegComb());
    
    return 1;
}

}
