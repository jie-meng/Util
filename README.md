## Util

[![Build Status](https://travis-ci.org/jie-meng/Util.svg?branch=master)](https://travis-ci.org/jie-meng/Util)

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
- json
- trace
- lua extension
- unittest

### How to build

- Install cmake
- Install mingw (On windows)

Build luaexec: execute command "python make.py" on terminal

luaexec is an extended lua script executor which contains basic lua & util extensions (platform, string-ext, file, memory, net, regex, csv, config, cmdline) which is provided by Util lib.

### How to get or extend  more useful lib written by lua

Run "./luaexec install_luaexeclib.lua" on terminal, it will install the libs in luaexeclib to lua system searching path. Then you can use the libs with "require". e.g.```local json = require('json')```, which is really easy to use and extend.

### How to do unittest

Run "python make_and_test.py". All cpp tests and lua test will run.

### How to test on linux if you are using MacOS or Windows

- Install docker
- Run "python linux_env.py", you'll be taken to a linux shell container where current dir volumed to container working path.
    - Run "python3 clear.py" in linux shell container to make working clean.
    - Run "python3 make_and_test.py" to do unittest on linux.