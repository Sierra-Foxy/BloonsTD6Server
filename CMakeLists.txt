cmake_minimum_required(VERSION 3.22)

project(BloonsTD6Tools
        VERSION 0.1.0
        DESCRIPTION "A set of tools and applications for BTD6"
        LANGUAGES CXX)

# Versions before *might* work, but they're not tested or supported, if you need older versions to work, you're on your own
set(minBoostVersion 1.81)
set(minOpenSSLVersion 3.0.2)
set(minZlibVersion 1.2.11)

include(FeatureSummary)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(CheckPackages)
include(CommonConfiguration)

add_subdirectory(src)
add_subdirectory(platform)

