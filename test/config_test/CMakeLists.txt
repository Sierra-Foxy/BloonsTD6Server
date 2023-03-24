include_directories(${PROJECT_SOURCE_DIR}/src/server/)

set(SHARED_SOURCES
        ${PROJECT_SOURCE_DIR}/src/server/api_config.cpp
)

set(SHARED_HEADERS
        ${PROJECT_SOURCE_DIR}/src/server/api_config.hpp
)

set(SOURCES
        load_test.cpp
)

add_executable(Config_Test_Run ${SOURCES} ${SHARED_SOURCES} ${SHARED_HEADERS})
target_link_libraries(Config_Test_Run ${Boost_LIBRARIES})

target_include_directories(Config_Test_Run PUBLIC ${CMAKE_SOURCE_DIR}/include/tomlplusplus)

add_test(NAME Config_Test
        COMMAND Config_Test_Run
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test/config_test)