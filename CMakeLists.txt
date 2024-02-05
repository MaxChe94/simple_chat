cmake_minimum_required(VERSION 3.5)

project(tcpChat LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Network)

option(BUILD_FOR_SERVER "Build for server" ON)

add_executable(server
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    connection.cpp
    connection.h
    server.cpp
    server.h
    client.cpp
    client.h
  )

if(BUILD_FOR_SERVER)
    target_compile_definitions(server PRIVATE BUILD_FOR_SERVER)
endif()

add_executable(client
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    connection.cpp
    connection.h
    server.cpp
    server.h
    client.cpp
    client.h
  )

if(NOT BUILD_FOR_SERVER)
    target_compile_definitions(client PRIVATE BUILD_FOR_SERVER)
endif()

target_link_libraries(
    server PRIVATE Qt5::Widgets
    Qt5::Network
    )

target_link_libraries(
    client PRIVATE Qt5::Widgets
    Qt5::Network
    )
