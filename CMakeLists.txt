cmake_minimum_required(VERSION 2.8)

set(APP_NAME luaexec)
project(${APP_NAME})

include_directories(
    src)
file(GLOB_RECURSE SRCS "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(${APP_NAME} ${SRCS})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	add_definitions(-D_CPP11_)
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	add_definitions(-D_CPP0X_)
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

if (UNIX)
    add_definitions(-D_PLATFORM_UNIX_)
    add_definitions(-DLUA_USE_POSIX)
    target_link_libraries(${APP_NAME} pthread)
elseif (WIN32)
    add_definitions(-D_PLATFORM_WINDOWS_)
    add_definitions(-DLUA_USE_WINDOWS)
    target_link_libraries(${APP_NAME} ws2_32 Shlwapi)
endif()

#cmake -DCMAKE_BUILD_TYPE=Debug ..
