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
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QThread>

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
    isRunning = false;
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
        QMainWindow { background-color: #040814; }
        QLabel { color: #00f3ff; font-size: 11px; font-weight: bold; font-family: 'Consolas', monospace; text-transform: uppercase; }
        QTableWidget {
            background-color: rgba(0, 20, 40, 0.55);
            color: #00f3ff;
            gridline-color: rgba(0, 243, 255, 0.3);
            border: 2px solid rgba(0, 243, 255, 0.6);
            border-radius: 8px;
            font-family: 'Consolas', monospace;
        }
        QHeaderView::section {
            background-color: rgba(0, 15, 30, 0.95);
            color: #ff007f;
            border: 1px solid rgba(0, 243, 255, 0.45);
            font-weight: bold;
            text-align: center;
        }
        QHeaderView::section:vertical {
            padding: 5px;
            min-width: 50px;
        }
        QTableCornerButton::section {
            background-color: rgba(0, 15, 30, 0.95);
            border: 1px solid rgba(0, 243, 255, 0.45);
        }
        QLineEdit {
            background-color: rgba(0, 15, 30, 0.85);
            color: #ff007f;
            border: 2px solid #ff007f;
            border-radius: 5px;
            padding: 3px;
            font-weight: bold;
            font-size: 14px;
        }
        QTextEdit {
            background-color: rgba(0, 20, 35, 0.7);
            color: #00ff99;
            border: 2px solid rgba(0, 255, 153, 0.5);
            border-radius: 8px;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 70, 120, 0.7), stop:1 rgba(0, 25, 45, 0.7));
            color: #00f3ff;
            border: 2px solid rgba(0, 243, 255, 0.6);
            border-radius: 4px;
            padding: 6px;
            font-weight: bold;
        }
        QPushButton:hover { background: rgba(0, 243, 255, 0.3); border: 2px solid #00f3ff; color: #fff; }
    )");

    QWidget* centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    memoryTable = new QTableWidget(16, 16, this);
    memoryTable->setHorizontalHeaderLabels({"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"});
    memoryTable->setVerticalHeaderLabels({"00","10","20","30","40","50","60","70","80","90","A0","B0","C0","D0","E0","F0"});
    memoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memoryTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memoryTable->verticalHeader()->setMinimumWidth(50);
    memoryTable->setShowGrid(true);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(new QLabel("🌐 HOLOGRAPHIC MEMORY MATRIX"));
    leftLayout->addWidget(memoryTable, 1);

    registerTable = new QTableWidget(16, 2, this);
    registerTable->setHorizontalHeaderLabels({"REG", "VALUE"});
    registerTable->verticalHeader()->setVisible(false);
    for (int i = 0; i < 16; i++) {
        QString regName = "R" + QString::number(i, 16).toUpper();
        registerTable->setItem(i, 0, new QTableWidgetItem(regName));
        registerTable->setItem(i, 1, new QTableWidgetItem("00"));
    }
    registerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    registerTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    registerTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    pcDisplay = new QLineEdit("00"); pcDisplay->setReadOnly(true); pcDisplay->setAlignment(Qt::AlignCenter);
    irDisplay = new QLineEdit("0000"); irDisplay->setReadOnly(true); irDisplay->setAlignment(Qt::AlignCenter);
    irDisplay->setStyleSheet("QLineEdit { color: #00f3ff; border-color: #00f3ff; }");

    QFormLayout* cpuLayout = new QFormLayout();
    cpuLayout->addRow(new QLabel("PC:"), pcDisplay);
    cpuLayout->addRow(new QLabel("IR:"), irDisplay);

    loadButton = new QPushButton("📂 LOAD PROGRAM");
    stepBackButton = new QPushButton("⏮️ STEP BACK");
    stepButton = new QPushButton("⏭️ SINGLE STEP");
    runButton = new QPushButton("▶️ EXECUTE ALL");
    resetButton = new QPushButton("🔄 SYSTEM RESET");

    loadButton->setStyleSheet("color: #ffcc00; border-color: #ffcc00;");
    stepBackButton->setStyleSheet("color: #ff9900; border-color: #ff9900;");
    stepButton->setStyleSheet("color: #00f3ff; border-color: #00f3ff;");
    runButton->setStyleSheet("color: #00ff99; border-color: #00ff99;");
    resetButton->setStyleSheet("color: #ff007f; border-color: #ff007f;");

    QVBoxLayout* btnLayout = new QVBoxLayout();
    btnLayout->addWidget(loadButton); btnLayout->addWidget(stepBackButton);
    btnLayout->addWidget(stepButton); btnLayout->addWidget(runButton);
    btnLayout->addWidget(resetButton);

    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->addWidget(new QLabel("⚡ CPU REGISTERS"));
    rightLayout->addWidget(registerTable, 1);
    rightLayout->addLayout(cpuLayout);
    rightLayout->addLayout(btnLayout);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addLayout(leftLayout, 67);
    topLayout->addLayout(rightLayout, 33);

    logOutput = new QTextEdit(); logOutput->setReadOnly(true); logOutput->setFixedHeight(120);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(topLayout, 1);
    mainLayout->addWidget(new QLabel("🖥️ REAL-TIME SYSTEM CONSOLE"));
    mainLayout->addWidget(logOutput);

    connect(loadButton, &QPushButton::clicked, this, &Window::onLoadClicked);
    connect(stepBackButton, &QPushButton::clicked, this, &Window::onStepBackClicked);
    connect(stepButton, &QPushButton::clicked, this, &Window::onStepClicked);
    connect(runButton, &QPushButton::clicked, this, &Window::onRunClicked);
    connect(resetButton, &QPushButton::clicked, this, &Window::onResetClicked);
}

void Window::updateUI() {
    string pcStr = simulator->getRegisterFile()->getPC();
    int pcAddr = 0;
    try { pcAddr = std::stoi(pcStr, nullptr, 16); } catch(...) { pcAddr = 0; }

    for (int r = 0; r < 16; r++) {
        for (int c = 0; c < 16; c++) {
            int idx = r * 16 + c;
            string val = simulator->getMemory()->readCell(idx);
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(val));
            item->setTextAlignment(Qt::AlignCenter);

            if (idx == pcAddr || idx == (pcAddr + 1) % 256) {
                item->setBackground(QBrush(QColor(255, 0, 127, 220)));
                item->setForeground(QBrush(QColor(255, 255, 255)));
                item->setFont(QFont("Consolas", 11, QFont::Bold));
            } else if (val != "00") {
                item->setBackground(QBrush(QColor(0, 243, 255, 95)));
                item->setForeground(QBrush(QColor(0, 243, 255)));
                item->setFont(QFont("Consolas", 11, QFont::Bold));
            } else {
                item->setBackground(QBrush(QColor(0, 0, 0, 0)));
                item->setForeground(QBrush(QColor(40, 75, 95)));
                item->setFont(QFont("Consolas", 11));
            }
            memoryTable->setItem(r, c, item);
        }
    }

    vector<string> regs = simulator->getRegisterFile()->getAllRegisters();
    for (int i = 0; i < 16; i++) {
        string regVal = (i < static_cast<int>(regs.size())) ? regs[i] : "00";
        QTableWidgetItem* resItem = new QTableWidgetItem(QString::fromStdString(regVal));
        resItem->setTextAlignment(Qt::AlignCenter);

        if (regVal != "00") {
            resItem->setBackground(QBrush(QColor(0, 255, 153, 200)));
            resItem->setForeground(QBrush(QColor(255, 255, 255)));
            resItem->setFont(QFont("Consolas", 11, QFont::Bold));
        } else {
            resItem->setBackground(QBrush(QColor(0, 0, 0, 0)));
            resItem->setForeground(QBrush(QColor(40, 75, 95)));
            resItem->setFont(QFont("Consolas", 11));
        }
        registerTable->setItem(i, 1, resItem);

        QTableWidgetItem* regNameItem = registerTable->item(i, 0);
        if (regNameItem) {
            regNameItem->setTextAlignment(Qt::AlignCenter);
            regNameItem->setBackground(QBrush(QColor(0, 15, 30, 200)));
            regNameItem->setForeground(QBrush(QColor(255, 0, 127)));
            regNameItem->setFont(QFont("Consolas", 11, QFont::Bold));
        }
    }

    registerTable->scrollToTop();
    pcDisplay->setText(QString::fromStdString(simulator->getRegisterFile()->getPC()));
    irDisplay->setText(QString::fromStdString(simulator->getRegisterFile()->getIR()));
}

void Window::logMessage(const QString& msg) { logOutput->append(">> " + msg); }

void Window::onLoadClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Vole Code File", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

        QString cleanText;
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.contains(';')) {
                line = line.left(line.indexOf(';'));
            }
            cleanText += line + " ";
        }
        file.close();

        QRegularExpression hexRegex("[0-9A-Fa-f]{2}");
        QRegularExpressionMatchIterator i = hexRegex.globalMatch(cleanText);
        vector<string> hexBytes;
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            hexBytes.push_back(match.captured(0).toUpper().toStdString());
        }

        if (hexBytes.empty()) {
            QMessageBox::critical(this, "Error", "NO VALID HEX INSTRUCTIONS FOUND IN FILE.");
            return;
        }

        resetSimulatorInstance();
        loadedFilePath = fileName.toStdString();

        for (size_t addr = 0; addr < 256; addr++) {
            if (addr < hexBytes.size()) {
                simulator->getMemory()->writeCell(static_cast<int>(addr), hexBytes[addr]);
            } else {
                simulator->getMemory()->writeCell(static_cast<int>(addr), "00");
            }
        }

        simulator->getRegisterFile()->setPC("00");
        simulator->getRegisterFile()->setIR("0000");
        captureInitialMemorySnapshot();
        logMessage("SUCCESS: Loaded " + QString::number(hexBytes.size()) + " bytes from " + fileName);
        updateUI();
    }
}

void Window::onStepClicked() {
    if (simulator->isProgramHalted()) {
        logMessage("WARNING: CPU is HALTED (C000 executed). Reset or load new code.");
        return;
    }
    if (currentStepCount == 0) captureInitialMemorySnapshot();

    string pcStr = simulator->getRegisterFile()->getPC();
    int pcAddr = 0;
    try { pcAddr = std::stoi(pcStr, nullptr, 16); } catch(...) { pcAddr = 0; }

    string c1 = simulator->getMemory()->readCell(pcAddr);
    string c2 = simulator->getMemory()->readCell((pcAddr + 1) % 256);
    if (c1 == "00" && c2 == "00") {
        logMessage("NOTE: Executing empty cell (0000) at PC = " + QString::fromStdString(pcStr));
    }

    simulator->executeNextStep();
    currentStepCount++;
    updateUI();
    logMessage("STEP " + QString::number(currentStepCount) + " | PC = " + QString::fromStdString(simulator->getRegisterFile()->getPC()));
}

void Window::onStepBackClicked() {
    if (currentStepCount <= 0) return;

    int targetStep = currentStepCount - 1;
    delete simulator;
    simulator = new Simulator();

    for (int i = 0; i < 256; i++) {
        simulator->getMemory()->writeCell(i, initialMemorySnapshot[i]);
    }

    simulator->getRegisterFile()->setPC("00");
    simulator->getRegisterFile()->setIR("0000");

    for (int i = 0; i < targetStep; i++) {
        if (!simulator->isProgramHalted()) {
            simulator->executeNextStep();
        }
    }

    currentStepCount = targetStep;
    updateUI();
    logMessage("STEP BACK: Returned to Step " + QString::number(currentStepCount));
}

void Window::onRunClicked() {
    if (simulator->isProgramHalted()) {
        logMessage("WARNING: Program is already halted. Reset to run again.");
        return;
    }
    if (currentStepCount == 0) captureInitialMemorySnapshot();

    logMessage("RUNNING PROGRAM WITH ANIMATED STEPPING...");
    isRunning = true;
    loadButton->setEnabled(false);
    stepBackButton->setEnabled(false);
    stepButton->setEnabled(false);
    runButton->setEnabled(false);

    int steps = 0;
    const int MAX_STEPS = 1000;

    while (isRunning && !simulator->isProgramHalted() && steps < MAX_STEPS) {
        simulator->executeNextStep();
        steps++;
        currentStepCount++;

        updateUI();
        QCoreApplication::processEvents();
        QThread::msleep(25);
    }

    bool wasRunning = isRunning;
    isRunning = false;

    loadButton->setEnabled(true);
    stepBackButton->setEnabled(true);
    stepButton->setEnabled(true);
    runButton->setEnabled(true);

    if (!wasRunning) {
        return;
    }

    updateUI();
    if (simulator->isProgramHalted()) {
        logMessage("EXECUTION FINISHED: Program Halted (C000 executed) in " + QString::number(steps) + " steps.");
    } else if (steps >= MAX_STEPS) {
        logMessage("STOPPED: Execution limit reached (1000 steps). Infinite loop detected.");
        QMessageBox::warning(this, "Warning", "Infinite loop detected or step limit reached.");
    } else {
        logMessage("EXECUTION FINISHED IN " + QString::number(steps) + " STEPS.");
    }
}

void Window::onResetClicked() {
    isRunning = false;
    delete simulator;
    simulator = new Simulator();
    currentStepCount = 0;
    loadedFilePath = "";
    initialMemorySnapshot.assign(256, "00");
    simulator->getRegisterFile()->setPC("00");
    simulator->getRegisterFile()->setIR("0000");

    loadButton->setEnabled(true);
    stepBackButton->setEnabled(true);
    stepButton->setEnabled(true);
    runButton->setEnabled(true);

    updateUI();
    logMessage("SYSTEM RESET COMPLETE: Execution stopped, Memory and Registers cleared.");
}

#include "Window.moc"