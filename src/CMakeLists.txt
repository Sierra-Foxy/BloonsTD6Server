include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(BTD6ToolsApp Cosmetics.h MultiplayerMessages.cpp MultiplayerMessages.h LobbyMessage.cpp LobbyMessage.h MessagePart.h Bloons.cpp Bloons.h Cosmetics.cpp ExternalMessage.cpp ExternalMessage.h GameConfig.h GameConfig.cpp)

target_sources(BTD6ToolsApp PRIVATE
        main.cpp
        ConfigFile.cpp
        ConfigFile.h
        common.h
        cgroupControl.cpp
        cgroupControl.h
        ConnectionManager.cpp
        ConnectionManager.h
        Connection.cpp
        Connection.h
        Proxy.cpp
        Proxy.h
        dgdata.cpp
        dgdata.h
        Message.cpp
        Message.h
        Hmac.cpp
        Hmac.h
        Response.cpp
        Response.h
        Request.cpp
        Request.h
        MessageTemplates.cpp
        MultiplayerServer.cpp
        MultiplayerServer.h
        ClientConnection.cpp
        ClientConnection.h
        Net.cpp
        Net.h
        GameInstance.cpp
        GameInstance.h
        Player.cpp
        Player.h
        Towers.h
        Towers.cpp
        MonkeyKnowledge.h)

target_link_libraries(BTD6ToolsApp
                      OpenSSL::Crypto OpenSSL::SSL
                      ZLIB::ZLIB
                      Boost::iostreams Boost::json)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(BTD6ToolsApp ws2_32)
endif()