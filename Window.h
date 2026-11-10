#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include "Simulator.h"
#include <string>
#include <vector>

using namespace std;

class Window : public QMainWindow {
    Q_OBJECT

private:
    Simulator* simulator;
    QTableWidget* memoryTable;
    QTableWidget* registerTable;
    QLineEdit* pcDisplay;
    QLineEdit* irDisplay;
    QTextEdit* logOutput;
    QPushButton* loadButton;
    QPushButton* stepBackButton;
    QPushButton* stepButton;
    QPushButton* runButton;
    QPushButton* resetButton;

    int currentStepCount;
    bool isRunning;
    string loadedFilePath;
    vector<string> initialMemorySnapshot;

    void setupUI();
    void updateUI();
    void logMessage(const QString& msg);
    void resetSimulatorInstance();
    void captureInitialMemorySnapshot();

private slots:
    void onLoadClicked();
    void onStepBackClicked();
    void onStepClicked();
    void onRunClicked();
    void onResetClicked();

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window() override;
};

#endif