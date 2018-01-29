import platform
import sys
import os
import os.path
import make

def getAppName():
    if platform.system().lower().find('windows') >= 0:
        return 'luaexec.exe'
    else:
        return './luaexec'

def unittest():
    print("UnitTest started ...\n")
    return os.system(getAppName() + " $")

def installLuaexeclib(ci = False):
    if ci:
        return os.system(getAppName() + " install_luaexeclib_ci.lua")
    else:
        return os.system(getAppName() + " install_luaexeclib.lua")

def luatest():
    return os.system(getAppName() + " lua_test/lua_test.lua")

if __name__ == "__main__":
    if os.path.isfile(getAppName()):
        print('Delete ' + getAppName())
        os.remove(getAppName())
    
    if make.make() != 0:
        sys.exit(-1)

    if unittest() != 0:
        sys.exit(-1)

    if installLuaexeclib(len(sys.argv) > 1 and sys.argv[1].lower() == 'ci') != 0:
        sys.exit(-1)
        
    if luatest() != 0:
        sys.exit(-1)

    sys.exit(0)
