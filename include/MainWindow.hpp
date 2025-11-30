#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "RobotFactory.hpp"
#include "StockData.hpp"

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void onStrategyChanged(int index);
    void onNextDayClicked();
    void onExecuteStratClicked();
    void onRunNDaysClicked();
    void onBuyClicked();
    void onSummaryClicked();

    void appendLog(const QString &text);

    // Backend
    AbstractRobotFactory *factory;
    AbstractRobot        *robot;
    StockMarket           stocks;

    // Widgets
    QComboBox   *strategyCombo;
    QPushButton *nextDayButton;
    QPushButton *executeStratButton;
    QPushButton *runNDaysButton;

    QSpinBox    *daysSpin;
    QLineEdit   *tickerEdit;
    QSpinBox    *qtySpin;
    QPushButton *buyButton;
    QPushButton *summaryButton;

    QTextEdit   *logView;
};

#endif
