#include "../include/MainWindow.hpp"
#include <QString>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      factory(nullptr),
      robot(nullptr)
{
    factory = new RobotFactory();
    robot = factory->createRobot();
    robot->getStockData(&stocks);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Strategy row
    QHBoxLayout *strategyLayout = new QHBoxLayout;
    strategyCombo = new QComboBox(this);
    strategyCombo->addItem("Basic (Strategy 1)");
    strategyCombo->addItem("Low Risk (Strategy 2)");
    strategyCombo->addItem("Random (Strategy 3)");
    strategyLayout->addWidget(strategyCombo);

    // Buttons row
    QHBoxLayout *buttonsLayout1 = new QHBoxLayout;
    nextDayButton      = new QPushButton("Next Day", this);
    executeStratButton = new QPushButton("Execute Strategy", this);
    runNDaysButton     = new QPushButton("Run N Days", this);
    buttonsLayout1->addWidget(nextDayButton);
    buttonsLayout1->addWidget(executeStratButton);
    buttonsLayout1->addWidget(runNDaysButton);

    // Run N days spinbox
    QHBoxLayout *runNLayout = new QHBoxLayout;
    daysSpin = new QSpinBox(this);
    daysSpin->setRange(1, 365);
    daysSpin->setValue(10);
    runNLayout->addWidget(new QLabel("Days:", this));
    runNLayout->addWidget(daysSpin);

    // Buy row
    QHBoxLayout *buyLayout = new QHBoxLayout;
    tickerEdit = new QLineEdit(this);
    qtySpin    = new QSpinBox(this);
    qtySpin->setRange(1, 100000);
    qtySpin->setValue(10);
    buyButton  = new QPushButton("Buy", this);

    buyLayout->addWidget(new QLabel("Ticker:", this));
    buyLayout->addWidget(tickerEdit);
    buyLayout->addWidget(new QLabel("Qty:", this));
    buyLayout->addWidget(qtySpin);
    buyLayout->addWidget(buyButton);

    // Sell row
    QHBoxLayout *sellLayout = new QHBoxLayout;
    sellTickerEdit = new QLineEdit(this);
    sellQtySpin    = new QSpinBox(this);
    sellQtySpin->setRange(1, 100000);
    sellQtySpin->setValue(10);
    sellButton     = new QPushButton("Sell", this);
    sellAllButton  = new QPushButton("Sell All", this);

    sellLayout->addWidget(new QLabel("Sell Ticker:", this));
    sellLayout->addWidget(sellTickerEdit);
    sellLayout->addWidget(new QLabel("Qty:", this));
    sellLayout->addWidget(sellQtySpin);
    sellLayout->addWidget(sellButton);
    sellLayout->addWidget(sellAllButton);


    // Summary button + log
    summaryButton = new QPushButton("Show Summary (console)", this);
    logView       = new QTextEdit(this);
    logView->setReadOnly(true);

    mainLayout->addLayout(strategyLayout);
    mainLayout->addLayout(buttonsLayout1);
    mainLayout->addLayout(runNLayout);
    mainLayout->addLayout(buyLayout);
    mainLayout->addLayout(sellLayout);
    mainLayout->addWidget(summaryButton);
    mainLayout->addWidget(new QLabel("Log:", this));
    mainLayout->addWidget(logView);

    central->setLayout(mainLayout);

    connect(strategyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onStrategyChanged);

    connect(nextDayButton, &QPushButton::clicked, this, &MainWindow::onNextDayClicked);
    connect(executeStratButton, &QPushButton::clicked, this, &MainWindow::onExecuteStratClicked);
    connect(runNDaysButton, &QPushButton::clicked, this, &MainWindow::onRunNDaysClicked);

    connect(buyButton, &QPushButton::clicked, this, &MainWindow::onBuyClicked);
    connect(sellButton, &QPushButton::clicked, this, &MainWindow::onSellClicked);
    connect(sellAllButton, &QPushButton::clicked, this, &MainWindow::onSellAllClicked);

    connect(summaryButton, &QPushButton::clicked, this, &MainWindow::onSummaryClicked);


    setWindowTitle("Stock Trading Robot GUI");
    resize(700, 500);

    // default strategy
    onStrategyChanged(0);
}

MainWindow::~MainWindow()
{
    delete robot;
    delete factory;
}

void MainWindow::appendLog(const QString &text)
{
    logView->append(text);
}

void MainWindow::onStrategyChanged(int index)
{
    if (!robot)
        return;
    robot->setStrategy(index);
    appendLog("Strategy changed to: " + strategyCombo->currentText());
}

void MainWindow::onNextDayClicked()
{
    stocks.updateDate();
    appendLog("Advanced one day.");
}

void MainWindow::onExecuteStratClicked()
{
    robot->executeStrat();
    appendLog("Executed strategy for current day.");
}

void MainWindow::onRunNDaysClicked()
{
    int days = daysSpin->value();
    for (int i = 0; i < days; ++i) {
        stocks.updateDate();
        robot->executeStrat();
    }
    appendLog(QString("Ran strategy for %1 days.").arg(days));
}

void MainWindow::onBuyClicked()
{
    QString qticker = tickerEdit->text();
    int qty = qtySpin->value();

    if (qticker.isEmpty()) {
        appendLog("Ticker empty; nothing bought.");
        return;
    }
    qticker = qticker.toUpper();
    std::string ticker = qticker.toStdString();

    robot->buy(qticker.toStdString(), qty);
    appendLog(QString("Manual buy: %1 x %2").arg(qticker).arg(qty));
}
void MainWindow::onSellClicked()
{
    if (!robot) return;

    QString qticker = sellTickerEdit->text();
    int qty = sellQtySpin->value();

    if (qticker.isEmpty()) {
        appendLog("Sell ticker is empty; nothing sold.");
        return;
    }
    qticker = qticker.toUpper();
    std::string ticker = qticker.toStdString();

    robot->sell(ticker, qty);

    appendLog(QString("Manual sell: %1 x %2").arg(qticker).arg(qty));
}

void MainWindow::onSellAllClicked()
{
    if (!robot) return;

    robot->sellAll();

    appendLog("Sell All: closed all positions.");
}


void MainWindow::onSummaryClicked()
{
    robot->summary();
    appendLog("Summary printed to console.");
}
