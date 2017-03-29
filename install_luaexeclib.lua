--[[If you are using windows, please run this script in a bash shell (Mingw/Cygwin)]]

print('Start install luaexeclib ...')

local idx = string.find(package.path, ';')
if not idx then
    print('Install luaexeclib failed.')
    os.exit(-1)
end

local first_dir = util.splitPathname(string.sub(package.path, 1, idx-1))
if util.isPathDir(first_dir) then
    print('Remove old luaexeclib files from path: ' .. first_dir)
    os.execute('rm ' .. first_dir .. '/*.*')
else
    print('Create path: ' .. first_dir)
    os.execute('mkdir -p ' .. first_dir)
end

print('Copy new luaexeclib files to path: ' .. first_dir)
os.execute('cp -r luaexeclib/. ' .. first_dir)

print('Install luaexeclib finished')
