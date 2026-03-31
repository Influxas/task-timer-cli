cmake_minimum_required(VERSION 3.19)
project(task-timer-cli LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(task-timer-cli
    main.cpp
    TimerState.h TimerState.cpp
    TimerRecord.h
    TimerLogic.h TimerLogic.cpp
    README.md
)

target_link_libraries(task-timer-cli
    PRIVATE
        Qt::Core
)

install(TARGETS task-timer-cli
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
