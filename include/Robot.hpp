#ifndef ROBOT_H
#define ROBOT_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <ctime>

#include "AbstractRobot.hpp"
#include "StockData.hpp"
#include "StrategyLowRisk.hpp"
#include "StrategyRandom.hpp"
#include "StrategyBasic.hpp"

using namespace std;

class Robot : public AbstractRobot {
    private:
        StockMarket *stocks;
        AbstractStrategy *strat;
        string strategyName;
        string tradeLogFile = "Trades Log.csv";
        string portfolioFile = "Trade History.txt";

    public:
        Robot() {
            balance = 100000;
            stockBalance = 0;
            stocks = nullptr;
            strat = nullptr;
        }

        void getStockData(StockMarket *data) {
            stocks = data;
        }

        void updateStockBalance() {
            stockBalance = 0;
            for (const auto &ticker : wallet) {
                double openPrice = stocks->marketData[ticker.first][stocks->date];
                int quantity = ticker.second;
                stockBalance += (openPrice * quantity);
            }
        }

        void sellAll() {
            for (const auto &ticker : wallet) {
                if (ticker.second > 0) {
                    sell(ticker.first, ticker.second);
                }
            }
            updateStockBalance();
        }

        void buy(string ticker, int quant) {
            // check price
            double openPrice = stocks->marketData[ticker][stocks->date];

            // check can afford
            if ((quant * openPrice) < balance) {
                balance -= (openPrice * quant);

                // record trade in memory
                portfolio.push_back({ticker, quant, openPrice, stocks->date, "BOUGHT"});

                // update wallet
                if (!wallet[ticker]) {
                    wallet[ticker] = quant;
                } else {
                    wallet[ticker] += quant;
                }

                cout << "Stock " << ticker << " BOUGHT " << quant << endl;

                // also log to CSV
                writeTradeToFile(ticker, quant, openPrice, stocks->date, "BOUGHT");
            }
        }

        void sell(string ticker, int quant) {
            double openPrice = stocks->marketData[ticker][stocks->date];

            // check has stock
            if (wallet[ticker] >= quant) {
                wallet[ticker] -= quant;
                balance += (openPrice * quant);

                // record trade in memory
                portfolio.push_back({ticker, quant, openPrice, stocks->date, "SOLD"});

                cout << "Stock " << ticker << " SOLD " << quant << endl;

                // also log to CSV
                writeTradeToFile(ticker, quant, openPrice, stocks->date, "SOLD");
            }
        }

        // Set trading strategy
        void setStrategy(int type) {
            if (strat) {
                if (stocks) {
                    stocks->detach(strat);
                }
                delete strat;
                strat = nullptr;
            }

            switch (type) {
            case 0:
                strat = new StrategyBasic();
                strategyName = "Basic";
                break;
            case 1:
                strat = new StrategyLowRisk();
                strategyName = "Low";
                break;
            case 2:
                strat = new StrategyRandom();
                strategyName = "Random";
                break;
            default:
                strat = new StrategyBasic();
                strategyName = "Basic";
                break;
            }

            if (stocks && strat) {
                stocks->attach(strat);
            }

            cout << "[Robot] Strategy set to " << strategyName << endl;
        }

        // Run current strategy for the day
        void executeStrat() {
            if (!strat || !stocks) {
                cout << "[Robot] Strategy or stocks not set!" << endl;
                return;
            }

            cout << "\n[Robot] Executing strategy '" << strategyName
                << "' for date " << stocks->date << endl;

            // type 0 = buy list
            vector<string> temp = strat->getStock(0);
            if (!temp.empty()) {
                for (int i = 0; i < temp.size(); i++) {
                    buy(temp[i], 1);
                }
            }

            // type 1 = sell list
            temp = strat->getStock(1);
            if (!temp.empty()) {
                for (int i = 0; i < temp.size(); i++) {
                    int quantity = wallet[temp[i]];
                    if (wallet[temp[i]] && (quantity > 0)) {
                        sell(temp[i], quantity);
                    }
                }
            }

            updateStockBalance();
        }

        // CSV logger for each trade
        void writeTradeToFile(const string &ticker,
                            int quantity,
                            double price,
                            const string &date,
                            const string &action)
        {
            ofstream file(tradeLogFile, ios::app);

            if (!file.is_open()) {
                cout << "Could not open trade log file: " << tradeLogFile << endl;
                return;
            }

            file << date   << "," << action << "," << ticker << "," << quantity  << "," << price  << endl;
        }

        // Print summary to console and build logSummary string
        void summary() {
            cout << "\n";
            cout << "Date: " << stocks->date << endl;
            cout << "=== BALANCE ===" << endl;
            cout << "Balance: " << balance << endl;
            cout << "Stocks: " << stockBalance << endl;

            cout << "=== CURRENT HOLDINGS ===" << endl;
            cout << left << setw(10) << "Ticker"
                << setw(10) << "Quantity" << endl;
            cout << string(20, '-') << endl;

            for (const auto &p : wallet) {
                if (p.second > 0) {
                    cout << left
                        << setw(10) << p.first
                        << setw(10) << p.second
                        << endl;
                }
            }

            // Build logSummary string for GUI
            stringstream logStream;
            logStream << "\n";
            logStream << "Date: " << stocks->date << endl;
            logStream << "=== BALANCE ===" << endl;
            logStream << "Balance: " << balance << endl;
            logStream << "$ in Stocks: " << stockBalance << endl;
            logStream << "Total Profit: " << ((balance + stockBalance) - 100000) << endl;
            logStream << "=== CURRENT HOLDINGS ===" << endl;

            logStream << left << setw(10) << "Ticker"
                    << setw(10) << "Quantity" << endl;
            logStream << string(20, '-') << endl;

            for (const auto &p : wallet) {
                if (p.second > 0) {
                    logStream << left
                            << setw(15) << p.first
                            << setw(15) << p.second
                            << endl;
                }
            }

            string logString = logStream.str();
            logSummary = logString;
        }

        // Write full trade history to a unique TXT file
        void printPortfolioToFile() {
            const char *filename = portfolioFile.c_str();

            FILE *f = fopen(filename, "w");
            if (!f) {
                cout << "Could not open " << filename << " for writing.\n";
                return;
            }

            // Header
            fprintf(f, "Ticker,Quantity,Price,Date,TransactionType\n");

            for (size_t i = 0; i < portfolio.size(); ++i) {
                const auto &transaction = portfolio[i];

                string ticker  = get<0>(transaction);
                int    quantity = get<1>(transaction);
                double price    = get<2>(transaction);
                string date     = get<3>(transaction);
                string action   = get<4>(transaction);

                fprintf(f, "%s,%d,%f,%s,%s\n",
                        ticker.c_str(),
                        quantity,
                        price,
                        date.c_str(),
                        action.c_str());
            }

            fclose(f);

            cout << "Trade history written to " << filename << endl;
        }
};

#endif
