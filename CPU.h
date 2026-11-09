
#ifndef CPU_H
#define CPU_H

#include "Memory.h"
#include "RegisterFile.h"
#include "ALU.h"
#include <string>

class CPU {
private:
    Memory* memory;
    RegisterFile* regFile;
    bool halted; // Flag to check if Opcode C000 was executed

public:
    // Constructor links the CPU with the existing Memory and Registers
    CPU(Memory* mem, RegisterFile* reg);

    // Core cycle
    void fetch();
    void decodeAndExecute();

    // State checker for Simulator
    bool isHalted();
    void resetHalt();
};

#endif
