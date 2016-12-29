#include "lutillib.hpp"
#include "util/luaextend.hpp"
#include "lplatformext.hpp"
#include "lstringext.hpp"
#include "lmemoryext.hpp"
#include "lregexext.hpp"
#include "lnetext.hpp"
#include "lthreadext.hpp"
#include "lfileext.hpp"
#include "lprocessext.hpp"
#include "lcsvext.hpp"
#include "lcfgext.hpp"

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
    extendNet(plua_state);
    extendProcess(plua_state);
    extendRegex(plua_state);
    extendThread(plua_state);
    
    luaCreateLib(plua_state, LuaRegCombUtilLib::getInstance().getRegComb());
    
    return 1;
}

}
