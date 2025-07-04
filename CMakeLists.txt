cmake_minimum_required(VERSION 3.19)
project(tourist_map LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(tourist_map
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    global.h
    infotip.h infotip.cpp
    mapview.h mapview.cpp
    newmapwindow.h newmapwindow.cpp
    node.h node.cpp
    road.h road.cpp
    spot.h spot.cpp
    spoteditor.h spoteditor.cpp
    touristmap.h touristmap.cpp
)

target_link_libraries(tourist_map
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS tourist_map
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET tourist_map
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
