dofile("./make.lua")

if string.find(string.lower(platformInfo()), "unix") then
    file.fileRename("Util", "luaexec")
elseif string.find(string.lower(platformInfo()), "windows") then
    file.fileRename("Util.exe", "luaexec.exe")
end

print("make luaexec ok")