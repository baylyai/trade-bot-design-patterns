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
#include <QLabel>

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
    void onSellClicked();
    void onSellAllClicked();

    void appendLog(const QString &text);

    // Backend
    AbstractRobotFactory *factory;
    AbstractRobot        *robot;
    StockMarket           stocks;

    QLabel *elapsedDaysLabel;
    QLabel *dateLabel;
    QComboBox   *strategyCombo;
    QPushButton *nextDayButton;
    QPushButton *executeStratButton;
    QPushButton *runNDaysButton;

    QSpinBox    *daysSpin;

    // Buy controls
    QLineEdit   *tickerEdit;
    QSpinBox    *qtySpin;
    QPushButton *buyButton;

    // Sell controls
    QLineEdit   *sellTickerEdit;
    QSpinBox    *sellQtySpin;
    QPushButton *sellButton;
    QPushButton *sellAllButton;
    QPushButton *summaryButton;

    QTextEdit   *logView;

};

#endif
