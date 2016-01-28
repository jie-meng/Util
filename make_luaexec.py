import os
import platform

if os.system("make.py") == 0:
    if platform.system().lower().find('windows') >= 0:
        os.rename('Util.exe', 'luaexec.exe')
    else:
        os.rename('Util', 'luaexec')
    print('Make luaexec ok.')
else:
    print('Make luaexec failed.')