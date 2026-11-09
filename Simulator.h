
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Memory.h"
#include "RegisterFile.h"
#include "CPU.h"
#include <string>

class Simulator {
private:
    Memory memory;
    RegisterFile regFile;
    CPU* cpu;

public:
    Simulator();
    ~Simulator();

    // File Parsing and Loading
    bool loadProgram(std::string filename, int startAddress); 
    
    // Execution Controls
    void executeNextStep(); // For "Single Step" button
    void runAll();          // For "Run" button
    
    // Getters for GUI to read the state without modifying it
    Memory* getMemory();
    RegisterFile* getRegisterFile();
    bool isProgramHalted();
};

#endif
