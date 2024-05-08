cmake_minimum_required(VERSION 3.14)

project(NetWork LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED Network)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED Network)

add_executable(NetWork
  main.cpp
)

target_link_libraries(NetWork
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
)
