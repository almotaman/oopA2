
#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <vector>
#include <string>

class RegisterFile {
private:
    std::vector<std::string> registers; // 16 general purpose registers (R0-RF)
    std::string pc; // Program Counter (2 hex chars)
    std::string ir; // Instruction Register (4 hex chars)

public:
    RegisterFile(); // Initialize registers to "00" and PC to "00", IR to "0000"

    // Setters & Getters
    std::string getRegister(int index);
    void setRegister(int index, std::string value);

    std::string getPC();
    void setPC(std::string value);

    std::string getIR();
    void setIR(std::string value);

    // GUI Helper
    std::vector<std::string> getAllRegisters();
};

#endif
