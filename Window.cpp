#include "Window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSizePolicy>

using namespace std;

Window::Window(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    updateUI();
}

void Window::setupUI() {
    this->setWindowTitle("⚡ VOLE MACHINE SIMULATOR - CS213");
    this->resize(1200, 750);
    this->setMinimumSize(950, 600);

    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #0f111a;
        }
        QLabel {
            color: #82aaff;
            font-size: 14px;
            font-weight: bold;
            font-family: 'Segoe UI', sans-serif;
            margin-bottom: 4px;
        }
        QTableWidget {
            background-color: #1a1c23;
            color: #eeffff;
            gridline-color: #2e3244;
            border: 1px solid #2e3244;
            border-radius: 8px;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 13px;
        }
        QHeaderView::section {
            background-color: #24283b;
            color: #ffcb6b;
            padding: 4px;
            border: 1px solid #2e3244;
            font-weight: bold;
            font-size: 12px;
        }
        QLineEdit {
            background-color: #090a10;
            color: #c3e88d;
            border: 1px solid #414868;
            border-radius: 6px;
            padding: 6px;
            font-family: 'Consolas', monospace;
            font-size: 14px;
            font-weight: bold;
        }
        QTextEdit {
            background-color: #090a10;
            color: #c3e88d;
            border: 1px solid #2e3244;
            border-radius: 8px;
            font-family: 'Consolas', monospace;
            font-size: 13px;
            padding: 8px;
        }
        QPushButton {
            background-color: #1f2335;
            color: #c0caf5;
            border: 1px solid #3b4261;
            border-radius: 6px;
            padding: 10px;
            font-size: 13px;
            font-weight: bold;
            font-family: 'Segoe UI', sans-serif;
        }
        QPushButton:hover {
            background-color: #3b4261;
            color: #ffffff;
            border-color: #7aa2f7;
        }
        QPushButton:pressed {
            background-color: #7aa2f7;
            color: #0f111a;
        }
    )");

    QWidget* centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    memoryTable = new QTableWidget(16, 16, this);
    memoryTable->setHorizontalHeaderLabels({"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"});
    memoryTable->setVerticalHeaderLabels({"00","10","20","30","40","50","60","70","80","90","A0","B0","C0","D0","E0","F0"});

    memoryTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    memoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memoryTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(new QLabel("💾 MEMORY GRID (256 CELLS)"));
    leftLayout->addWidget(memoryTable, 1);

    registerTable = new QTableWidget(16, 2, this);
    registerTable->setHorizontalHeaderLabels({"REG", "VALUE"});
    for (int i = 0; i < 16; i++) {
        QString regName = "R" + QString::number(i, 16).toUpper();
        registerTable->setItem(i, 0, new QTableWidgetItem(regName));
        registerTable->setItem(i, 1, new QTableWidgetItem("00"));
    }
    registerTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    registerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    registerTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    pcDisplay = new QLineEdit("00");
    pcDisplay->setReadOnly(true);
    pcDisplay->setAlignment(Qt::AlignCenter);

    irDisplay = new QLineEdit("0000");
    irDisplay->setReadOnly(true);
    irDisplay->setAlignment(Qt::AlignCenter);

    QFormLayout* cpuInfoLayout = new QFormLayout();
    QLabel* pcLabel = new QLabel("PC:"); pcLabel->setStyleSheet("color: #f7768e;");
    QLabel* irLabel = new QLabel("IR:"); irLabel->setStyleSheet("color: #bb9af7;");
    cpuInfoLayout->addRow(pcLabel, pcDisplay);
    cpuInfoLayout->addRow(irLabel, irDisplay);

    loadButton = new QPushButton("📂 Load Program");
    loadButton->setStyleSheet("QPushButton { border-left: 4px solid #7aa2f7; }");

    stepButton = new QPushButton("⏭️ Single Step");
    stepButton->setStyleSheet("QPushButton { border-left: 4px solid #e0af68; }");

    runButton = new QPushButton("▶️ Run All");
    runButton->setStyleSheet("QPushButton { border-left: 4px solid #9ece6a; background-color: #1a2f23; }");

    resetButton = new QPushButton("🔄 Reset Simulator");
    resetButton->setStyleSheet("QPushButton { border-left: 4px solid #f7768e; }");

    QVBoxLayout* btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(8);
    btnLayout->addWidget(loadButton);
    btnLayout->addWidget(stepButton);
    btnLayout->addWidget(runButton);
    btnLayout->addWidget(resetButton);

    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->addWidget(new QLabel("⚙️ CPU REGISTERS"));
    rightLayout->addWidget(registerTable, 1);
    rightLayout->addSpacing(10);
    rightLayout->addLayout(cpuInfoLayout);
    rightLayout->addSpacing(10);
    rightLayout->addLayout(btnLayout);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addLayout(leftLayout, 65);
    topLayout->addSpacing(15);
    topLayout->addLayout(rightLayout, 35);

    logOutput = new QTextEdit();
    logOutput->setReadOnly(true);
    logOutput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->addLayout(topLayout, 75);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("🖥️ EXECUTION LOG / CONSOLE"));
    mainLayout->addWidget(logOutput, 25);

    connect(loadButton, &QPushButton::clicked, this, &Window::onLoadClicked);
    connect(stepButton, &QPushButton::clicked, this, &Window::onStepClicked);
    connect(runButton, &QPushButton::clicked, this, &Window::onRunClicked);
    connect(resetButton, &QPushButton::clicked, this, &Window::onResetClicked);

    logMessage("SYSTEM READY. LOAD A INSTRUCTION FILE TO BEGIN.");
}

void Window::updateUI() {
    vector<string> mem = simulator.getMemory()->getAllMemory();
    for (int r = 0; r < 16; r++) {
        for (int c = 0; c < 16; c++) {
            int idx = r * 16 + c;
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(mem[idx]));
            item->setTextAlignment(Qt::AlignCenter);
            memoryTable->setItem(r, c, item);
        }
    }

    vector<string> regs = simulator.getRegisterFile()->getAllRegisters();
    for (int i = 0; i < 16; i++) {
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(regs[i]));
        item->setTextAlignment(Qt::AlignCenter);
        item->setForeground(QBrush(QColor("#c3e88d")));
        registerTable->setItem(i, 1, item);

        QTableWidgetItem* regNameItem = registerTable->item(i, 0);
        if (regNameItem) {
            regNameItem->setTextAlignment(Qt::AlignCenter);
            regNameItem->setForeground(QBrush(QColor("#7aa2f7")));
        }
    }

    pcDisplay->setText(QString::fromStdString(simulator.getRegisterFile()->getPC()));
    irDisplay->setText(QString::fromStdString(simulator.getRegisterFile()->getIR()));
}

void Window::logMessage(const QString& msg) {
    logOutput->append(">> " + msg);
}

void Window::onLoadClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Vole Code File", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty()) {
        if (simulator.loadProgram(fileName.toStdString(), 0)) {
            logMessage("SUCCESS: Program loaded from " + fileName);
            updateUI();
        } else {
            QMessageBox::critical(this, "Error", "Failed to load program file.");
        }
    }
}

void Window::onStepClicked() {
    if (simulator.isProgramHalted()) {
        logMessage("WARNING: CPU is HALTED.");
        return;
    }
    simulator.executeNextStep();
    updateUI();
    logMessage("STEP EXECUTED. PC = " + QString::fromStdString(simulator.getRegisterFile()->getPC()));
}

void Window::onRunClicked() {
    if (simulator.isProgramHalted()) {
        logMessage("WARNING: CPU is HALTED. Reset or load new code.");
        return;
    }

    string pcStr = simulator.getRegisterFile()->getPC();
    int pcAddr = 0;
    try { pcAddr = std::stoi(pcStr, nullptr, 16); } catch(...) { pcAddr = 0; }

    string inst1 = simulator.getMemory()->readCell(pcAddr);
    string inst2 = simulator.getMemory()->readCell(pcAddr + 1);

    if (inst1 == "00" && inst2 == "00") {
        logMessage("ERROR: Memory is empty (0000). Load a program (.txt) first!");
        QMessageBox::warning(this, "Empty Memory", "Please load a program file (.txt) before clicking 'Run All'!");
        return;
    }

    logMessage("RUNNING PROGRAM...");

    int steps = 0;
    const int MAX_STEPS = 5000;

    while (!simulator.isProgramHalted() && steps < MAX_STEPS) {
        simulator.executeNextStep();
        steps++;

        if (steps % 10 == 0) {
            QCoreApplication::processEvents();
        }
    }

    updateUI();

    if (steps >= MAX_STEPS) {
        logMessage("STOPPED: Execution limit reached (" + QString::number(MAX_STEPS) + " steps). Possible infinite loop.");
        QMessageBox::warning(this, "Infinite Loop Warning", "Stopped: Program took more than 5000 steps without HALT (C000).");
    } else {
        logMessage("RUN COMPLETED: Executed " + QString::number(steps) + " steps until HALT.");
    }
}

void Window::onResetClicked() {
    simulator.getMemory()->clearMemory();
    simulator.getRegisterFile()->setPC("00");
    simulator.getRegisterFile()->setIR("0000");
    updateUI();
    logMessage("SYSTEM RESET: Memory and Registers cleared.");
}