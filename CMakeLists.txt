cmake_minimum_required(VERSION 3.25)
project(oopA2)

set(CMAKE_CXX_STANDARD 17)

add_executable(oopA2
        main.cpp
        Simulator.cpp
        Simulator.h
        CPU.cpp
        CPU.h
        ALU.cpp
        ALU.h
        RegisterFile.cpp
        RegisterFile.h
        Memory.h
        Memory.cpp
)