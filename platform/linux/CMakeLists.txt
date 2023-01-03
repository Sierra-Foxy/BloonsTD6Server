if (NOT SYSTEMD_SERVICE_INSTALL_DIR)
    find_package(Systemd)
    if (NOT SYSTEMD_FOUND)
        message(FATAL_ERROR
                "Could not find systemd service install directory."
                "Pass the -DSYSTEMD_SERVICE_INSTALL_DIR=/path/to/systemd/services option"
                " or install systemd pkg-config"
                )
    endif()
endif()

set(FULL_CGROUP_SCRIPT_DIR ${CMAKE_INSTALL_FULL_DATADIR})
set(CGROUP_ROOT ${CMAKE_INSTALL_FULL_DATADIR}/btd6server/cgroup-root)
configure_file(systemd/btd6cgroupctrl.service.in ${CMAKE_CURRENT_BINARY_DIR}/btd6cgroupctrl.service @ONLY)
configure_file(systemd/btd6cgroupctrl.socket.in ${CMAKE_CURRENT_BINARY_DIR}/btd6cgroupctrl.socket @ONLY)
configure_file(cgroup/onStart.sh.in ${CMAKE_CURRENT_BINARY_DIR}/onStart.sh @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/btd6cgroupctrl.service" "${CMAKE_CURRENT_BINARY_DIR}/btd6cgroupctrl.socket"
        DESTINATION ${SYSTEMD_SERVICE_INSTALL_DIR}
        COMPONENT data
        )

install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/onStart.sh" "cgroup/cgroup.py"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/btd6server
        COMPONENT data
        )

