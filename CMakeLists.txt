cmake_minimum_required(VERSION 3.24)
if (${CMAKE_BINARY_DIR} EQUAL ${CMAKE_SOURCE_DIR})
    message(FATAL_ERROR "Do not build in-source. Create a build directory")
endif()

project(BloonsTools
        DESCRIPTION "A set of tools for BloonsTD6"
        LANGUAGES CXX)

include(GNUInstallDirs)

include(cmake/file_locations.cmake)

set(minOpenSSLVersion 3.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_ALL "Builds all executables and files" ON)
option(BUILD_SERVER "Builds server executable" OFF)
option(SELF_TEST "Runs tests" OFF)

option(STATIC_BINARY "Build static binaries" OFF)
option(STATIC_LIBRARY "Build static libraries" OFF)

set(BUILD_SHARED_LIBS ON)
if (${STATIC_LIBRARY})
    set(BUILD_SHARED_LIBS OFF)
endif()

if (${STATIC_BINARY})
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

add_subdirectory(src)

if(SELF_TEST)
    enable_testing()
    add_subdirectory(test)
endif()

#add_executable(BloonsTools main.cpp)
