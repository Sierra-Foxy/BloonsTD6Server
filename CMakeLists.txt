cmake_minimum_required(VERSION 3.24)
project(BloonsTools
        DESCRIPTION "A set of tools for BloonsTD6"
        LANGUAGES CXX)

option(BUILD_ALL "Builds all executables and files" ON)
option(BUILD_SERVER "Builds server executable" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)

#add_executable(BloonsTools main.cpp)
