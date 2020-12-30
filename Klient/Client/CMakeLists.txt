cmake_minimum_required(VERSION 3.5)

project(Client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Core Widgets Network Multimedia REQUIRED)

if(ANDROID)
  add_library(Client SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(Client
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
endif()
target_link_libraries(Client Qt5::Widgets Qt5::Network Qt5::Multimedia)

