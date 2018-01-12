function forceCopyFile(src, dst)
    local path, _ = util.splitPathname(dst)
    if not (util.strEndWith(path, '.svn') or util.strEndWith(path, '.git')) then
        if not util.isPathDir(path) then
            util.mkDir(path)
        end
        util.fileCopy(src, dst, false)    
    end
end

print('Start install luaexeclib ...')

local idx = string.find(package.path, ';')
if not idx then
    print('Install luaexeclib failed.')
    os.exit(-1)
end

local first_dir = util.splitPathname(string.sub(package.path, 1, idx-1))
print('Remove old luaexeclib files from path: ' .. first_dir)
util.pathRemoveAll(first_dir)
util.mkDir(first_dir)

print('Copy new luaexeclib files to path: ' .. first_dir)
local files = util.findFilesInDirRecursively(util.currentPath() .. '/luaexeclib')
for _, v in ipairs(files) do
    local dest = util.strReplace(v, util.currentPath() .. '/luaexeclib', first_dir)
    print(string.format('Copy "%s"  ---->  "%s"', v, dest))
    forceCopyFile(v, dest)
end

print('Install luaexeclib finished')
