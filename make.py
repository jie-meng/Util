import os
import shutil
import platform

if platform.system().lower().find('windows') >= 0:
    os.system('cmake -G "MinGW Makefiles"')
    os.system('mingw32-make')
else:
    os.system("cmake .")
    os.system("make")

print("Make ok.")
