local application = './Util'
local cmake_cmd = 'cmake .'
local make_cmd = 'make'

if strContains(platformInfo(), "win", false) then
    application = 'Util.exe'
    cmake_cmd = 'cmake -G "MinGW Makefiles'
    make_cmd = 'mingw32-make'
end

print('Make started ...\n')

file.pathRemove(application)
os.execute(cmake_cmd)
os.execute(make_cmd)

if not file.isPathFile(application) then
    print('Make fail.\n')
    os.exit(-1)
end

print('Make success.\n')

print("Run app started ...\n")
os.execute(application)
print("\n\nRun app stopped.\n")
