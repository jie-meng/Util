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

if __name__ == "__main__":
    if os.path.isfile(getAppName()):
        print('Delete ' + getAppName())
        os.remove(getAppName())
    
    if make.make() != 0:
        sys.exit(-1)

    sys.exit(unittest())