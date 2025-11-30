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

    // Summary button + log
    summaryButton = new QPushButton("Show Summary (console)", this);
    logView       = new QTextEdit(this);
    logView->setReadOnly(true);

    mainLayout->addLayout(strategyLayout);
    mainLayout->addLayout(buttonsLayout1);
    mainLayout->addLayout(runNLayout);
    mainLayout->addLayout(buyLayout);
    mainLayout->addWidget(summaryButton);
    mainLayout->addWidget(new QLabel("Log:", this));
    mainLayout->addWidget(logView);

    central->setLayout(mainLayout);

    QObject::connect(
        strategyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [this](int index) { onStrategyChanged(index); });

    QObject::connect(
        nextDayButton, &QPushButton::clicked,
        this, [this]() { onNextDayClicked(); });

    QObject::connect(
        executeStratButton, &QPushButton::clicked,
        this, [this]() { onExecuteStratClicked(); });

    QObject::connect(
        runNDaysButton, &QPushButton::clicked,
        this, [this]() { onRunNDaysClicked(); });

    QObject::connect(
        buyButton, &QPushButton::clicked,
        this, [this]() { onBuyClicked(); });

    QObject::connect(
        summaryButton, &QPushButton::clicked,
        this, [this]() { onSummaryClicked(); });

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
    // combo index 0,1,2 -> strategies 1,2,3
    int stratNumber = index + 1;
    robot->setStrategy(stratNumber);
    appendLog(QString("Strategy set to %1").arg(stratNumber));
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

    robot->buy(qticker.toStdString(), qty);
    appendLog(QString("Manual buy: %1 x %2").arg(qticker).arg(qty));
}

void MainWindow::onSummaryClicked()
{
    robot->summary();
    appendLog("Summary printed to console.");
}
