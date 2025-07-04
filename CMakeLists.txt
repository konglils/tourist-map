cmake_minimum_required(VERSION 3.19)
project(tourist_map VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 静态链接
set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib)
set(BUILD_SHARED_LIBS OFF)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Gui)

qt_standard_project_setup()

include_directories(src/core src/models src/ui)

# 递归收集源文件
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
)

# 创建可执行文件
add_executable(tourist_map ${SOURCES})

target_link_libraries(tourist_map Qt6::Core Qt::Widgets)

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

# O3 编译
add_compile_options(-O3)
