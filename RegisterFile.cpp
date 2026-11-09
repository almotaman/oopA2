#include "RegisterFile.h"
#include <stdexcept>

RegisterFile::RegisterFile() {
    registers.resize(16, "00");
    pc = "00";
    ir = "0000";
}

std::string RegisterFile::getRegister(int index) {
    if (index < 0 || index >= 16) {
        return "00";
    }
    return registers[index];
}

void RegisterFile::setRegister(int index, std::string value) {
    if (index >= 0 && index < 16) {
        if (value.length() == 1) {
            value = "0" + value;
        }
        registers[index] = value;
    }
}

std::string RegisterFile::getPC() {
    return pc;
}

void RegisterFile::setPC(std::string value) {
    if (value.length() == 1) {
        value = "0" + value;
    }
    pc = value;
}

std::string RegisterFile::getIR() {
    return ir;
}

void RegisterFile::setIR(std::string value) {
    ir = value;
}

std::vector<std::string> RegisterFile::getAllRegisters() {
    return registers;
}