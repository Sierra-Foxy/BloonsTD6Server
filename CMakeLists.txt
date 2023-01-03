cmake_minimum_required(VERSION 3.22)

project(BloonsTD6Server)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

add_subdirectory(platform)

find_package(Boost 1.81 COMPONENTS iostreams json REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

link_libraries(ssl crypto z ${Boost_LIBRARIES})

include_directories(/usr/local/boost_1_81_0/)
include_directories(src)

add_executable(BloonsTD6Server src/main.cpp src/ConfigFile.cpp src/ConfigFile.h src/common.h src/cgroupControl.cpp src/cgroupControl.h src/ConnectionManager.cpp src/ConnectionManager.h src/Connection.cpp src/Connection.h src/Proxy.cpp src/Proxy.h src/dgdata.cpp src/dgdata.h src/ApiEndpoint.cpp src/ApiEndpoint.h src/Hmac.cpp src/Hmac.h)