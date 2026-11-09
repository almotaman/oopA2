
#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <vector>
#include <string>

class Memory {
private:
    std::vector<std::string> memory; // 256 cells, each is a 2-char string (e.g., "00", "FF")

public:
    Memory(); // Constructor to initialize 256 cells with "00"
    
    // Core functions for CPU
    std::string readCell(int address); 
    void writeCell(int address, std::string value);
    
    // GUI and Simulator helpers
    std::vector<std::string> getAllMemory(); // Returns the whole memory for the GUI grid
    void clearMemory(); // Resets all cells to "00"
};

#endif
