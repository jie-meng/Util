## Util ##

A ligthweight cross-platform C++ library. 

Support Unix & Windows. 

Currently can run on MacOS/Linux/Windows.

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
- process
- regex
- trace
- lua extension
- unittest

### How to build ###

- Install cmake
- Install python3.x or python2.x (On windows)
- Install mingw (On windows)

Build luaexec: execute command "python make.py" on terminal

luaexec is an extended lua script executor which contains basic lua & util extensions (platform, string-ext, file, memory, net, regex, csv, config, cmdline) which is provided by Util lib.

### How to get or extend  more useful lib written by lua ###

Run "./luaexec install_luaexeclib.lua" on terminal, it will install the libs in luaexeclib to lua system searching path. Then you can use the libs with "require". e.g.```local json = require('json')```, which is really easy to use and extend.

