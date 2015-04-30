file.pathRemove("Util")
file.pathRemove("Util.exe")
file.pathRemove("luaexec")
file.pathRemove("luaexec.exe")
file.pathRemove("Makefile")
file.pathRemove("CMakeCache.txt")
file.pathRemove("cmake_install.cmake")
file.pathRemoveAll("CMakeFiles")

file.pathRemoveAll("Debug")
file.pathRemoveAll("Release")
file.pathRemoveAll("debug")
file.pathRemoveAll("release")

print("clean ok")

