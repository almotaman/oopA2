#include "Window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSizePolicy>
#include <QLabel>
#include <QBrush>
#include <QColor>
#include <QFont>

using namespace std;

Window::Window(QWidget *parent) : QMainWindow(parent) {
    simulator = new Simulator();
    memoryTable = nullptr;
    registerTable = nullptr;
    pcDisplay = nullptr;
    irDisplay = nullptr;
    logOutput = nullptr;
    loadButton = nullptr;
    stepBackButton = nullptr;
    stepButton = nullptr;
    runButton = nullptr;
    resetButton = nullptr;
    currentStepCount = 0;
    loadedFilePath = "";

    initialMemorySnapshot.assign(256, "00");

    setupUI();
    updateUI();
}

Window::~Window() {
    delete simulator;
}

void Window::resetSimulatorInstance() {
    delete simulator;
    simulator = new Simulator();
    currentStepCount = 0;
}

void Window::captureInitialMemorySnapshot() {
    initialMemorySnapshot.clear();
    for (int i = 0; i < 256; i++) {
        initialMemorySnapshot.push_back(simulator->getMemory()->readCell(i));
    }
}

void Window::setupUI() {
    this->setWindowTitle("CYBERPUNK VOLE SIMULATOR - HOLOGRAPHIC MATRIX 3D");
    this->resize(1350, 860);

    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #040814;
        }
        QLabel {
            color: #00f3ff;
            font-size: 11px;
            font-weight: bold;
            font-family: 'Consolas', monospace;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        QTableWidget {
            background-color: rgba(0, 20, 40, 0.55);
            color: #00f3ff;
            gridline-color: rgba(0, 243, 255, 0.3);
            border: 2px solid rgba(0, 243, 255, 0.6);
            border-radius: 8px;
            font-family: 'Consolas', monospace;
            font-size: 11px;
        }
        QHeaderView::section {
            background-color: rgba(0, 15, 30, 0.95);
            color: #ff007f;
            padding: 4px;
            border: 1px solid rgba(0, 243, 255, 0.45);
            font-weight: bold;
            font-size: 11px;
        }
        QHeaderView::section:horizontal {
            background-color: rgba(0, 15, 30, 0.95);
            color: #ff007f;
            border: 1px solid rgba(0, 243, 255, 0.45);
        }
        QHeaderView::section:vertical {
            background-color: rgba(0, 15, 30, 0.95);
            color: #ff007f;
            border: 1px solid rgba(0, 243, 255, 0.45);
            padding-left: 10px;
            padding-right: 10px;
        }
        QTableCornerButton::section {
            background-color: rgba(0, 15, 30, 0.95);
            border: 1px solid rgba(0, 243, 255, 0.45);
        }
        QLineEdit {
            background-color: rgba(0, 15, 30, 0.8);
            color: #ff007f;
            border: 2px solid #ff007f;
            border-radius: 4px;
            padding: 2px;
            font-family: 'Consolas', monospace;
            font-size: 13px;
            font-weight: bold;
        }
        QTextEdit {
            background-color: rgba(0, 20, 35, 0.7);
            color: #00ff99;
            border: 2px solid rgba(0, 255, 153, 0.5);
            border-radius: 8px;
            font-family: 'Consolas', monospace;
            font-size: 11px;
            padding: 4px;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 70, 120, 0.7), stop:1 rgba(0, 25, 45, 0.7));
            color: #00f3ff;
            border: 2px solid rgba(0, 243, 255, 0.6);
            border-radius: 4px;
            padding: 5px;
            font-size: 11px;
            font-weight: bold;
            font-family: 'Consolas', monospace;
        }
        QPushButton:hover {
            background: rgba(0, 243, 255, 0.3);
            border: 2px solid #00f3ff;
            color: #ffffff;
        }
        QPushButton:pressed {
            background-color: #00f3ff;
            color: #040814;
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
    memoryTable->verticalHeader()->setMinimumWidth(50);
    memoryTable->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    memoryTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    memoryTable->setShowGrid(true);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(4);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->addWidget(new QLabel("🌐 HOLOGRAPHIC MEMORY MATRIX"));
    leftLayout->addWidget(memoryTable, 1);

    registerTable = new QTableWidget(16, 2, this);
    registerTable->setHorizontalHeaderLabels({"REG", "VALUE"});
    registerTable->verticalHeader()->setVisible(false);
    registerTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    registerTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for (int i = 0; i < 16; i++) {
        QString regName = "R" + QString::number(i, 16).toUpper();
        registerTable->setItem(i, 0, new QTableWidgetItem(regName));
        registerTable->setItem(i, 1, new QTableWidgetItem("00"));
    }
    registerTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    registerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    registerTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    registerTable->setShowGrid(true);

    pcDisplay = new QLineEdit("00");
    pcDisplay->setReadOnly(true);
    pcDisplay->setAlignment(Qt::AlignCenter);

    irDisplay = new QLineEdit("0000");
    irDisplay->setReadOnly(true);
    irDisplay->setAlignment(Qt::AlignCenter);
    irDisplay->setStyleSheet("QLineEdit { color: #00f3ff; border-color: #00f3ff; }");

    QFormLayout* cpuInfoLayout = new QFormLayout();
    cpuInfoLayout->setSpacing(3);
    cpuInfoLayout->setContentsMargins(0, 0, 0, 0);
    QLabel* pcLabel = new QLabel("PROGRAM COUNTER (PC):"); pcLabel->setStyleSheet("color: #ff007f;");
    QLabel* irLabel = new QLabel("INSTRUCTION REG (IR):"); irLabel->setStyleSheet("color: #00f3ff;");
    cpuInfoLayout->addRow(pcLabel, pcDisplay);
    cpuInfoLayout->addRow(irLabel, irDisplay);

    loadButton = new QPushButton("📂 LOAD PROGRAM");
    stepBackButton = new QPushButton("⏮️ STEP BACK");
    stepButton = new QPushButton("⏭️ SINGLE STEP");
    runButton = new QPushButton("▶️ EXECUTE ALL");
    resetButton = new QPushButton("🔄 SYSTEM RESET");

    loadButton->setStyleSheet("border-color: #ffcc00; color: #ffcc00;");
    stepBackButton->setStyleSheet("border-color: #ff9900; color: #ff9900;");
    stepButton->setStyleSheet("border-color: #00f3ff; color: #00f3ff;");
    runButton->setStyleSheet("border-color: #00ff99; color: #00ff99;");
    resetButton->setStyleSheet("border-color: #ff007f; color: #ff007f;");

    QVBoxLayout* btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(3);
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->addWidget(loadButton);
    btnLayout->addWidget(stepBackButton);
    btnLayout->addWidget(stepButton);
    btnLayout->addWidget(runButton);
    btnLayout->addWidget(resetButton);

    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(4);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->addWidget(new QLabel("⚡ CPU REGISTERS"));
    rightLayout->addWidget(registerTable, 58);
    rightLayout->addLayout(cpuInfoLayout);
    rightLayout->addLayout(btnLayout);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing(10);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addLayout(leftLayout, 67);
    topLayout->addLayout(rightLayout, 33);

    logOutput = new QTextEdit();
    logOutput->setReadOnly(true);
    logOutput->setFixedHeight(110);
    logOutput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(5);
    mainLayout->addLayout(topLayout, 1);
    mainLayout->addWidget(new QLabel("🖥️ REAL-TIME SYSTEM CONSOLE"));
    mainLayout->addWidget(logOutput);

    connect(loadButton, &QPushButton::clicked, this, &Window::onLoadClicked);
    connect(stepBackButton, &QPushButton::clicked, this, &Window::onStepBackClicked);
    connect(stepButton, &QPushButton::clicked, this, &Window::onStepClicked);
    connect(runButton, &QPushButton::clicked, this, &Window::onRunClicked);
    connect(resetButton, &QPushButton::clicked, this, &Window::onResetClicked);

    logMessage("SYSTEM INITIALIZED. READY TO LOAD PROGRAM OR EXECUTE STEP.");
}

void Window::updateUI() {
    string pcStr = simulator->getRegisterFile()->getPC();
    int pcAddr = 0;
    try {
        pcAddr = std::stoi(pcStr, nullptr, 16);
    } catch(...) {
        pcAddr = 0;
    }

    for (int r = 0; r < 16; r++) {
        for (int c = 0; c < 16; c++) {
            int idx = r * 16 + c;
            string val = simulator->getMemory()->readCell(idx);
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(val));
            item->setTextAlignment(Qt::AlignCenter);

            if (idx == pcAddr || idx == (pcAddr + 1) % 256) {
                item->setBackground(QBrush(QColor(255, 0, 127, 220)));
                item->setForeground(QBrush(QColor(255, 255, 255)));
                item->setFont(QFont("Consolas", 10, QFont::Bold));
            } else if (val != "00") {
                item->setBackground(QBrush(QColor(0, 243, 255, 95)));
                item->setForeground(QBrush(QColor(0, 243, 255)));
                item->setFont(QFont("Consolas", 10, QFont::Bold));
            } else {
                item->setBackground(QBrush(QColor(0, 0, 0, 0)));
                item->setForeground(QBrush(QColor(40, 75, 95)));
            }
            memoryTable->setItem(r, c, item);
        }
    }

    vector<string> regs = simulator->getRegisterFile()->getAllRegisters();
    for (int i = 0; i < 16; i++) {
        string regVal = (i < static_cast<int>(regs.size())) ? regs[i] : "00";
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(regVal));
        item->setTextAlignment(Qt::AlignCenter);

        if (regVal != "00") {
            item->setBackground(QBrush(QColor(0, 255, 153, 100)));
            item->setForeground(QBrush(QColor(0, 255, 153)));
            item->setFont(QFont("Consolas", 10, QFont::Bold));
        } else {
            item->setBackground(QBrush(QColor(0, 0, 0, 0)));
            item->setForeground(QBrush(QColor(40, 75, 95)));
        }
        registerTable->setItem(i, 1, item);

        QTableWidgetItem* regNameItem = registerTable->item(i, 0);
        if (regNameItem) {
            regNameItem->setTextAlignment(Qt::AlignCenter);
            regNameItem->setForeground(QBrush(QColor(255, 0, 127)));
            regNameItem->setFont(QFont("Consolas", 10, QFont::Bold));
        }
    }

    registerTable->scrollToTop();
    pcDisplay->setText(QString::fromStdString(simulator->getRegisterFile()->getPC()));
    irDisplay->setText(QString::fromStdString(simulator->getRegisterFile()->getIR()));
}

void Window::logMessage(const QString& msg) {
    logOutput->append(">> " + msg);
}

void Window::onLoadClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Vole Code File", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty()) {
        resetSimulatorInstance();
        loadedFilePath = fileName.toStdString();
        if (simulator->loadProgram(loadedFilePath, 0)) {
            simulator->getRegisterFile()->setPC("00");
            simulator->getRegisterFile()->setIR("0000");
            captureInitialMemorySnapshot();
            logMessage("SUCCESS: Program loaded from " + fileName);
            updateUI();
        } else {
            QMessageBox::critical(this, "Error", "Failed to load program file.");
        }
    }
}

void Window::onStepClicked() {
    if (simulator->isProgramHalted()) {
        logMessage("WARNING: CPU is HALTED. Reset or load new code.");
        return;
    }

    if (currentStepCount == 0) {
        captureInitialMemorySnapshot();
    }

    simulator->executeNextStep();
    currentStepCount++;
    updateUI();
    QCoreApplication::processEvents();
    logMessage("SINGLE STEP EXECUTED (Step " + QString::number(currentStepCount) + "). PC = " + QString::fromStdString(simulator->getRegisterFile()->getPC()));
}

void Window::onStepBackClicked() {
    if (currentStepCount <= 0) return;

    int targetStep = currentStepCount - 1;
    resetSimulatorInstance();

    for (int i = 0; i < 256; i++) {
        simulator->getMemory()->writeCell(i, initialMemorySnapshot[i]);
    }

    simulator->getRegisterFile()->setPC("00");
    simulator->getRegisterFile()->setIR("0000");

    for (int i = 0; i < targetStep; i++) {
        if (!simulator->isProgramHalted()) {
            simulator->executeNextStep();
            currentStepCount++;
        }
    }

    updateUI();
    QCoreApplication::processEvents();
    logMessage("STEP BACK EXECUTED (Returned to Step " + QString::number(currentStepCount) + ").");
}

void Window::onRunClicked() {
    if (simulator->isProgramHalted()) return;

    if (currentStepCount == 0) {
        captureInitialMemorySnapshot();
    }

    logMessage("RUNNING PROGRAM...");
    int steps = 0;
    const int MAX_STEPS = 5000;

    while (!simulator->isProgramHalted() && steps < MAX_STEPS) {
        simulator->executeNextStep();
        steps++;
        currentStepCount++;
        if (steps % 10 == 0) {
            QCoreApplication::processEvents();
        }
    }

    updateUI();
    QCoreApplication::processEvents();
    if (steps >= MAX_STEPS) {
        logMessage("STOPPED: Infinite loop detected.");
        QMessageBox::warning(this, "Warning", "Infinite loop detected.");
    } else {
        logMessage("EXECUTION FINISHED IN " + QString::number(steps) + " STEPS.");
    }
}

void Window::onResetClicked() {
    resetSimulatorInstance();
    loadedFilePath = "";
    initialMemorySnapshot.assign(256, "00");
    simulator->getRegisterFile()->setPC("00");
    simulator->getRegisterFile()->setIR("0000");
    updateUI();
    QCoreApplication::processEvents();
    logMessage("SYSTEM RESET COMPLETE: Memory and Registers cleared.");
}

