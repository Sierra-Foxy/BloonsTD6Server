find_package(OpenSSL ${minOpenSSLVersion} REQUIRED)

project(Server
        LANGUAGES C CXX)

add_executable(Server)

target_compile_definitions(Server PRIVATE
        USER_CONFIG_DIR="${USER_CONFIG_DIR}/BloonsTools")

target_include_directories(Server PUBLIC ${CMAKE_SOURCE_DIR}/external/tomlplusplus)
target_include_directories(Server PUBLIC ${CMAKE_SOURCE_DIR}/src/)

target_sources(Server PUBLIC
        main.cpp
        api_proxy.hpp
        api_proxy.cpp
        api_config.hpp
        api_config.cpp
        listener.hpp
        listener.cpp
        session.hpp
        session.cpp
        error.hpp
        error.cpp
)

target_link_libraries(Server
        OpenSSL::Crypto OpenSSL::SSL)
