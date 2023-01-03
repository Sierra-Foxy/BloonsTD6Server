find_package(PkgConfig QUIET REQUIRED)

if (NOT SYSTEMD_FOUND)
    pkg_check_modules(SYSTEMD systemd)
endif()

if (SYSTEMD_FOUND AND "${SYSTEMD_SERVICE_INSTALL_DIR}" STREQUAL "")
    execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE}
            --variable=systemdsystemunitdir systemd
            OUTPUT_VARIABLE SYSTEMD_SERVICE_INSTALL_DIR)
    string(REGEX REPLACE "[ \t\n]+" "" SYSTEMD_SERVICE_INSTALL_DIR
            "${SYSTEMD_SERVICE_INSTALL_DIR}")
endif()

if (SYSTEMD_FOUND)
    message(STATUS "Systemd service install directory: ${SYSTEMD_SERVICE_INSTALL_DIR}")
endif()