
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include "Simulator.h"

using namespace std;

int main() {
    Simulator sim;
    string inputPath;

    cout << "==========================================" << endl;
    cout << "       VOLE SIMULATOR - TERMINAL TEST     " << endl;
    cout << "==========================================" << endl;
    cout << "Enter program file path or name: ";
    getline(cin, inputPath);

    ifstream inFile(inputPath);
    if (!inFile.is_open()) {
        cout << "ERROR: Could not open file " << inputPath << endl;
        return 1;
    }

    vector<string> hexBytes;
    string word;
    while (inFile >> word) {
        if (word.find(";") != string::npos) {
            word = word.substr(0, word.find(";"));
        }
        string cleanWord = "";
        for (char c : word) {
            if (isxdigit(static_cast<unsigned char>(c))) {
                cleanWord += static_cast<char>(toupper(c));
            }
        }
        if (cleanWord.length() == 4) {
            hexBytes.push_back(cleanWord.substr(0, 2));
            hexBytes.push_back(cleanWord.substr(2, 2));
        }
    }
    inFile.close();

    if (hexBytes.empty()) {
        cout << "ERROR: No valid 4-digit Hex instructions found." << endl;
        return 1;
    }

    for (int i = 0; i < 256; i++) {
        sim.getMemory()->writeCell(i, "00");
    }

    for (size_t i = 0; i < hexBytes.size() && i < 256; i++) {
        sim.getMemory()->writeCell(static_cast<int>(i), hexBytes[i]);
    }
    sim.getRegisterFile()->setPC("00");
    sim.getRegisterFile()->setIR("0000");

    cout << "\n[+] Successfully loaded " << hexBytes.size() << " bytes into memory." << endl;
    cout << "[+] Executing program..." << endl;

    sim.runAll();

    cout << "\n==========================================" << endl;
    cout << "             FINAL RESULTS                " << endl;
    cout << "==========================================" << endl;
    cout << "PC : " << sim.getRegisterFile()->getPC() << endl;
    cout << "IR : " << sim.getRegisterFile()->getIR() << endl;

    cout << "\n--- CPU REGISTERS (R0 - RF) ---" << endl;
    vector<string> regs = sim.getRegisterFile()->getAllRegisters();
    for (size_t i = 0; i < regs.size(); i++) {
        cout << "R" << hex << uppercase << i << ": " << regs[i] << "\t";
        if ((i + 1) % 4 == 0) cout << endl;
    }

    cout << "\n--- MEMORY CELL 00 ---" << endl;
    cout << "Memory[00] = " << sim.getMemory()->readCell(0) << endl;
    cout << "==========================================" << endl;

    return 0;
}

