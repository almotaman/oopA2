
#ifndef ALU_H
#define ALU_H

#include <string>

class ALU {
public:
    // Arithmetic Operations (Opcode 5, 6)
    static std::string addTwosComplement(std::string val1, std::string val2);
    static std::string addFloatingPoint(std::string val1, std::string val2);

    // Logical Operations (Opcode 7, 8, 9, A)
    static std::string bitwiseOR(std::string val1, std::string val2);
    static std::string bitwiseAND(std::string val1, std::string val2);
    static std::string bitwiseXOR(std::string val1, std::string val2);
    static std::string rotateRight(std::string val, int steps);

    // Jump condition Helper (Opcode B)
    static bool checkEqual(std::string val1, std::string val2);
};

#endif
