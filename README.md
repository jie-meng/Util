## Util ##

A ligth-weight cross platform (Unix & Windows) C++ library

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

Build luaexec: 

1. Build Util

2. rename Util to luaexec

luaexec is a lua script executor which contains basic lua & extensions (file, memory, net, process, regex, thread, csv, matrix)