#include "Simulator.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

Simulator::Simulator() {
    cpu = new CPU(&memory, &regFile);
}

Simulator::~Simulator() {
    delete cpu;
}


static std::string toUpperCase(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::toupper(static_cast<unsigned char>(c));
    }
    return result;
}


static bool isValidHex(const std::string& str, size_t expectedLength) {
    if (str.length() != expectedLength) return false;
    for (char c : str) {
        if (!std::isxdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}


static bool isValidInstruction(const std::string& inst) {
    if (!isValidHex(inst, 4)) return false;
    
    char opcode = std::toupper(inst[0]);

    if ((opcode >= '1' && opcode <= '9') || 
        (opcode >= 'A' && opcode <= 'C') || 
        (opcode >= 'a' && opcode <= 'c')) {
        return true;
    }

    if (opcode == '0' && toUpperCase(inst) == "0000") {
        return true;
    }
    
    return false;
}

bool Simulator::loadProgram(std::string filename, int startAddress) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    std::string word;
    int currentAddress = startAddress;

    while (file >> word) {
        word = toUpperCase(word);
        

        if (word.length() == 4) {
            if (!isValidInstruction(word)) {
                std::cerr << "Validation Error: Invalid instruction -> " << word << std::endl;
                file.close();
                return false;
            }
            if (currentAddress >= 0 && currentAddress + 1 < 256) {
                memory.writeCell(currentAddress, word.substr(0, 2));
                memory.writeCell(currentAddress + 1, word.substr(2, 2));
                currentAddress += 2;
            } else {
                std::cerr << "Error: Memory overflow during loading." << std::endl;
                file.close();
                return false;
            }
        } 
        else if (word.length() == 2) {
            if (!isValidHex(word, 2)) {
                std::cerr << "Validation Error: Invalid memory value -> " << word << std::endl;
                file.close();
                return false;
            }
            if (currentAddress >= 0 && currentAddress < 256) {
                memory.writeCell(currentAddress, word);
                currentAddress += 1;
            } else {
                std::cerr << "Error: Memory overflow during loading." << std::endl;
                file.close();
                return false;
            }
        } else {
            std::cerr << "Validation Error: Invalid token length -> " << word << std::endl;
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}

void Simulator::executeNextStep() {
    if (cpu->isHalted()) {
        return;
    }


    cpu->fetch();
    

    std::string currentIR = regFile.getIR();
    if (!isValidInstruction(currentIR) && currentIR != "0000") {
        std::cerr << "Validation Error: Attempted to execute invalid instruction -> " << currentIR << std::endl;

    }

    cpu->decodeAndExecute();
}

void Simulator::runAll() {

    while (!cpu->isHalted()) {
        executeNextStep();
    }
}

Memory* Simulator::getMemory() {
    return &memory;
}

RegisterFile* Simulator::getRegisterFile() {
    return &regFile;
}

bool Simulator::isProgramHalted() {
    return cpu->isHalted();
}