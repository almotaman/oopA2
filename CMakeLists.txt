cmake_minimum_required(VERSION 3.16)
project(oopA2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.12.0/mingw_64")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(oopA2
        main.cpp
        Window.h
        Window.cpp
        Memory.h
        Memory.cpp
        RegisterFile.h
        RegisterFile.cpp
        ALU.h
        ALU.cpp
        CPU.h
        CPU.cpp
        Simulator.h
        Simulator.cpp
)

target_link_libraries(oopA2 PRIVATE Qt6::Widgets)