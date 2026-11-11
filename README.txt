========================================================================
             VOLE MACHINE SIMULATOR - CYBERPUNK GUI
========================================================================

------------------------------------------------------------------------
1. PROJECT OVERVIEW
------------------------------------------------------------------------
This project is a modern, responsive Cyberpunk-themed Graphical User
Interface (GUI) for the Vole Machine Simulator, built using C++ and Qt6
Widgets. The application simulates the execution of Vole machine code
with real-time memory matrix visualization, register tracking, snapshot-
based step undo functionality, and non-blocking visual animations.

------------------------------------------------------------------------
2. USER INTERFACE & COMPONENT LAYOUT
------------------------------------------------------------------------
The interface is split into key visual components:

[1] Holographic Memory Matrix (16x16 Grid):
    - Represents the full 256 bytes of Vole main memory (Addresses 00 to FF).
    - Rows are labeled from 00 to F0, and columns from 0 to F.
    - Highlights current instruction cells pointed to by PC in pink.
    - Cell values glow with a neon cyan aesthetic.

[2] CPU Registers (R0 - RF):
    - Displays all 16 general-purpose registers simultaneously without scrolling.
    - Non-zero values automatically glow in neon green.
    - Displays PC (Program Counter) and IR (Instruction Register).

[3] Interactive Control Panel:
    - LOAD PROGRAM: Parses Vole assembly/hex text files and strips comments starting with ';'.
    - SINGLE STEP: Executes one instruction per click and updates memory/registers.
    - STEP BACK: Restores the exact memory and register state of the previous step.
    - EXECUTE ALL: Runs the program with real-time graphical updates.
    - SYSTEM RESET: Stops execution immediately, clears memory/registers, and resets PC to 00.

[4] Real-Time System Console:
    - Logs execution steps, file load status, and warnings.

------------------------------------------------------------------------
3. HOW TO INSTALL & RUN THE PROGRAM
------------------------------------------------------------------------
1. Requirements: C++20 Compiler (MinGW/GCC), CMake 3.16+, Qt 6.x installed.
2. Open the project in CLion or Qt Creator.
3. Configure CMake with Qt6 path (e.g., C:/Qt/6.12.0/mingw_64).
4. Build and Run the executable target.
5. Click "LOAD PROGRAM" to load a Vole instructions text file.