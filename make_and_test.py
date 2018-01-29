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

def installLuaexeclib():
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

    if installLuaexeclib() != 0:
        sys.exit(-1)

    print('ls lua')
    os.system('ls /usr/local/share/lua/5.3')
        
    if luatest() != 0:
        sys.exit(-1)

    sys.exit(0)
