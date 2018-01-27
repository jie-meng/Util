import os
import sys
import shutil
import platform

def make():
    ret = -1
    if platform.system().lower().find('windows') >= 0:
        os.system('cmake -G "MinGW Makefiles"')
        ret = os.system('mingw32-make')

    else:
        os.system("cmake .")
        ret = os.system("make")

    return ret

if __name__ == "__main__":
    sys.exit(make())
