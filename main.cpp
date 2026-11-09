
#include <iostream>
#include <string>
#include <iomanip>
#include "Simulator.h"

using namespace std;


void printMenu() {
    cout << "\n=====================================\n";
    cout << "      VOLE MACHINE SIMULATOR         \n";
    cout << "=====================================\n";
    cout << "1. Load Program from File\n";
    cout << "2. Execute Next Step (Single Step)\n";
    cout << "3. Run All (Execute until Halt)\n";
    cout << "4. Display Registers, PC, IR\n";
    cout << "5. Display Memory\n";
    cout << "6. Exit\n";
    cout << "=====================================\n";
    cout << "Enter your choice: ";
}


void printMemoryGrid(Memory* memory) {
    cout << "\n      ";
    for (int i = 0; i < 16; i++) {
        cout << uppercase << hex << i << "   ";
    }
    cout << dec << "\n-------------------------------------------------------------------\n";

    vector<string> allMem = memory->getAllMemory();
    for (int row = 0; row < 16; row++) {
        cout << uppercase << hex << row << "0 | ";
        for (int col = 0; col < 16; col++) {
            int index = row * 16 + col;
            cout << allMem[index] << "  ";
        }
        cout << "\n";
    }
    cout << dec;
}


void printRegisterStatus(RegisterFile* regFile) {
    cout << "\n--- CPU State ---\n";
    cout << "PC: " << regFile->getPC() << " | IR: " << regFile->getIR() << "\n\n";
    cout << "Registers:\n";

    vector<string> regs = regFile->getAllRegisters();
    for (int i = 0; i < 16; i++) {
        cout << "R" << uppercase << hex << i << ": " << regs[i] << "  ";
        if ((i + 1) % 4 == 0) cout << "\n";
    }
    cout << dec;
}

int main() {
    Simulator sim;
    int choice = 0;

    while (choice != 6) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                string filename;
                int startAddress = 0;
                cout << "Enter program file name (e.g., instructions.txt): ";
                cin >> filename;
                cout << "Enter start address (0-255, default 0): ";
                cin >> startAddress;

                if (sim.loadProgram(filename, startAddress)) {
                    cout << "Program loaded successfully!\n";
                } else {
                    cout << "Failed to load program. Check file path.\n";
                }
                break;
            }
            case 2: {
                if (sim.isProgramHalted()) {
                    cout << "Program is halted! Load a new program.\n";
                } else {
                    sim.executeNextStep();
                    cout << "Executed one instruction.\n";
                    printRegisterStatus(sim.getRegisterFile());
                }
                break;
            }
            case 3: {
                if (sim.isProgramHalted()) {
                    cout << "Program is already halted!\n";
                } else {
                    cout << "Running all instructions...\n";
                    sim.runAll();
                    cout << "Execution finished.\n";
                    printRegisterStatus(sim.getRegisterFile());
                }
                break;
            }
            case 4: {
                printRegisterStatus(sim.getRegisterFile());
                break;
            }
            case 5: {
                printMemoryGrid(sim.getMemory());
                break;
            }
            case 6: {
                cout << "Exiting Simulator. Goodbye!\n";
                break;
            }
            default:
                cout << "Invalid choice! Please select 1-6.\n";
        }
    }

    return 0;
}