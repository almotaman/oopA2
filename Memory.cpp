#include "Memory.h"
#include <cctype>

using namespace std;

Memory::Memory() : memory(256, "00") {
}

string Memory::readCell(int cellIndex) {
    if (cellIndex < 0 || cellIndex >= static_cast<int>(memory.size())) {
        cerr << "Invalid memory address: " << cellIndex << '\n';
        return "00";
    }

    return memory[cellIndex];
}

void Memory::writeCell(int cellIndex, string hexValue) {
    if (cellIndex < 0 || cellIndex >= static_cast<int>(memory.size())) {
        cerr << "Invalid memory address: " << cellIndex << '\n';
        return;
    }

    if (hexValue.empty() || hexValue.size() > 2) {
        cerr << "Invalid memory value: " << hexValue << '\n';
        return;
    }

    for (char &digit : hexValue) {
        unsigned char current = static_cast<unsigned char>(digit);

        if (!isxdigit(current)) {
            cerr << "Invalid hex value: " << hexValue << '\n';
            return;
        }

        digit = static_cast<char>(toupper(current));
    }

    if (hexValue.size() == 1) {
        hexValue = "0" + hexValue;
    }

    memory[cellIndex] = hexValue;
}

vector<string> Memory::getAllMemory() {
    return memory;
}

void Memory::clearMemory() {
    for (string &cell : memory) {
        cell = "00";
    }
}