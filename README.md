## Util ##

Cross-platform (Unix & Windows) C++ library

- any type
- typeinfo
- lexicalcast
- std::string extension
- filesystem
- config file
- net
- thread
- timer
- csv
- matrix
- process
- regex
- trace
- lua extension
- unittest

### How to build ###

- Install cmake
- Install python3.x or python2.x (On windows)
- Install mingw (On windows)

Build Util: Go to directory of Util, execute command: python make.py

Build luaexec: Go to directory of Util, execute command: python make_luaexec.py

### Difference of platform ###

#### unix ####


DEFINES = \_PLATFORM\_UNIX\_

LIBS = -lpthread


#### windows ####

DEFINES = \_PLATFORM\_WINDOWS\_

LIBS = -lws2_32 -lShlwapi
