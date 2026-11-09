#include "CPU.h"
#include <sstream>
#include <iomanip>


CPU::CPU(Memory* mem, RegisterFile* reg) {
    this->memory = mem;
    this->regFile = reg;
    this->halted = false;
}


void CPU::fetch() {
    if (halted) return;
    std::string pcHex = regFile->getPC();
    int pcInt = std::stoi(pcHex, nullptr, 16);

    std::string byte1 = memory->readCell(pcInt);
    std::string byte2 = memory->readCell((pcInt + 1) % 256); // Wrap around if necessary

    regFile->setIR(byte1 + byte2);

    pcInt = (pcInt + 2) % 256;

    std::stringstream ss;
    ss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << pcInt;
    regFile->setPC(ss.str());
}


void CPU::decodeAndExecute() {
    if (halted) return;

    std::string ir = regFile->getIR();
    if (ir.length() != 4) return;

    char opcode = ir[0];


    int regR = std::stoi(std::string(1, ir[1]), nullptr, 16);


    std::string xy = ir.substr(2, 2);
    int addressXY = std::stoi(xy, nullptr, 16);

    switch (opcode) {
        case '1': {
            std::string memValue = memory->readCell(addressXY);
            regFile->setRegister(regR, memValue);
            break;
        }
        case '2': {
            regFile->setRegister(regR, xy);
            break;
        }
        case '3': {
            std::string regValue = regFile->getRegister(regR);
            memory->writeCell(addressXY, regValue);
            break;
        }
        case '4': {

            int sourceReg = std::stoi(std::string(1, ir[2]), nullptr, 16);
            int destReg = std::stoi(std::string(1, ir[3]), nullptr, 16);
            std::string val = regFile->getRegister(sourceReg);
            regFile->setRegister(destReg, val);
            break;
        }
        case '5': {

            int regS = std::stoi(std::string(1, ir[2]), nullptr, 16);
            int regT = std::stoi(std::string(1, ir[3]), nullptr, 16);
            std::string valS = regFile->getRegister(regS);
            std::string valT = regFile->getRegister(regT);
            std::string result = ALU::addTwosComplement(valS, valT);
            regFile->setRegister(regR, result);
            break;
        }
        case '6': {

            int regS = std::stoi(std::string(1, ir[2]), nullptr, 16);
            int regT = std::stoi(std::string(1, ir[3]), nullptr, 16);
            std::string valS = regFile->getRegister(regS);
            std::string valT = regFile->getRegister(regT);
            std::string result = ALU::addFloatingPoint(valS, valT);
            regFile->setRegister(regR, result);
            break;
        }
        case '7': {


            int regS = std::stoi(std::string(1, ir[2]), nullptr, 16);
            int regT = std::stoi(std::string(1, ir[3]), nullptr, 16);


            std::string valS = regFile->getRegister(regS);
            std::string valT = regFile->getRegister(regT);
            std::string result = ALU::bitwiseOR(valS, valT);
            regFile->setRegister(regR, result);
            break;
        }
        case '8': {


            int regS = std::stoi(std::string(1, ir[2]), nullptr, 16);
            int regT = std::stoi(std::string(1, ir[3]), nullptr, 16);
            std::string valS = regFile->getRegister(regS);
            std::string valT = regFile->getRegister(regT);
            std::string result = ALU::bitwiseAND(valS, valT);
            regFile->setRegister(regR, result);
            break;
        }
        case '9': {


            int regS = std::stoi(std::string(1, ir[2]), nullptr, 16);
            int regT = std::stoi(std::string(1, ir[3]), nullptr, 16);

            std::string valS = regFile->getRegister(regS);
            std::string valT = regFile->getRegister(regT);
            std::string result = ALU::bitwiseXOR(valS, valT);
            regFile->setRegister(regR, result);
            break;
        }
        case 'A':
        case 'a': {


            int steps = std::stoi(std::string(1, ir[3]), nullptr, 16);
            std::string valR = regFile->getRegister(regR);
            std::string result = ALU::rotateRight(valR, steps);
            regFile->setRegister(regR, result);
            break;
        }
        case 'B':
        case 'b': {
            std::string valR = regFile->getRegister(regR);
            std::string valR0 = regFile->getRegister(0);
            if (ALU::checkEqual(valR, valR0)) {
                regFile->setPC(xy); // Update PC to jump address
            }
            break;
        }
        case 'C':
        case 'c': {


            if (ir == "C000" || ir == "c000") {
                halted = true;
            }
            break;
        }
        default:
            break;
    }
}


bool CPU::isHalted() {
    return halted;
}

void CPU::resetHalt() {
    halted = false;
}