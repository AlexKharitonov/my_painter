cmake_minimum_required(VERSION 3.5)

project(main LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        resourses/resources.qrc)

add_executable(main
    ${PROJECT_SOURCES})

target_include_directories(main PRIVATE ../MyPainter)

target_link_libraries(main PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(main PRIVATE MyPainter)

