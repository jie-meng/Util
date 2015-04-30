if string.find(string.lower(platformInfo()), "unix") then
    os.execute("cmake .")
    os.execute("make")
elseif string.find(string.lower(platformInfo()), "windows") then
    os.execute([[cmake -G "MinGW Makefiles"]])
    os.execute("mingw32-make")
end

print("make ok")