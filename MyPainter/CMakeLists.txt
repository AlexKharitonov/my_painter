cmake_minimum_required(VERSION 3.5)

project(MyPainter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        MyPainter.cpp
        MyPainter.h
        MyPainter.ui        
)


add_library(MyPainter STATIC
            ${PROJECT_SOURCES})

target_link_libraries(MyPainter PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
