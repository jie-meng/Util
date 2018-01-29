print('CI Start install luaexeclib ...')

local idx = string.find(package.path, ';')
if not idx then
    print('Install luaexeclib failed.')
    os.exit(-1)
end

local first_dir = util.splitPathname(string.sub(package.path, 1, idx-1))
print('Remove old luaexeclib files from path: ' .. first_dir)
os.execute('sudo rm -rf ' .. first_dir)
os.execute('sudo mkdir -p ' .. first_dir)

print('Copy new luaexeclib files to path: ' .. first_dir)
os.execute('sudo cp -r luaexeclib/. ' .. first_dir)

print('Install luaexeclib finished')
