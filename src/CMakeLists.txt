cmake_minimum_required(VERSION 3.16)
project(trade-bot-design-patterns LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 10.1 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

add_executable(trade-bot-design-patterns
    WIN32 MACOSX_BUNDLE
    main.cpp MainWindow.cpp MainWindow.hpp)

target_link_libraries(trade-bot-design-patterns
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS trade-bot-design-patterns
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET LoginForm
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})