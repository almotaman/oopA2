#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include "Simulator.h"

class Window : public QMainWindow {
    Q_OBJECT

private:
    Simulator simulator;
    QTableWidget* memoryTable;
    QTableWidget* registerTable;
    QLineEdit* pcDisplay;
    QLineEdit* irDisplay;
    QTextEdit* logOutput;
    QPushButton* loadButton;
    QPushButton* stepButton;
    QPushButton* runButton;
    QPushButton* resetButton;

    void setupUI();
    void updateUI();
    void logMessage(const QString& msg);

private slots:
    void onLoadClicked();
    void onStepClicked();
    void onRunClicked();
    void onResetClicked();

public:
    Window(QWidget *parent = nullptr);
    ~Window() = default;
};

#endif