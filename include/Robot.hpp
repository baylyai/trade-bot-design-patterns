#ifndef ROBOT_H
#define ROBOT_H
#include <unordered_map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <sstream>
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
        std::string strategyName;
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
            for(const auto& ticker : wallet) {
                double openPrice = stocks->marketData[ticker.first][stocks->date];
                int quantity = ticker.second;
                stockBalance += (openPrice * quantity);
            }
        }
        void sellAll() {
            for(const auto& ticker : wallet) {
                if(ticker.second > 0)
                    sell(ticker.first, ticker.second);
            }
            updateStockBalance();
        }
        void buy(string ticker, int quant) {
            // check price
            double openPrice = stocks->marketData[ticker][stocks->date];

            // check can afford
            if((quant * openPrice) < balance) {
                balance -= (openPrice * quant);
                //stockBalance += (openPrice * quant);
                // keeping track of history
                portfolio.push_back({ticker, quant, openPrice, stocks->date, "BOUGHT"});
                if(!wallet[ticker]) {
                    wallet[ticker] = quant;
                } else {
                    wallet[ticker] += quant;
                }
                cout << "Stock " << ticker << " BOUGHT " << quant << endl;
            }

        }

        void sell(string ticker, int quant) {
            double openPrice = stocks->marketData[ticker][stocks->date];
            // check has stock
            if(wallet[ticker] >= quant) {
                wallet[ticker] -= quant;
                balance += (openPrice * quant);
                //stockBalance -= (openPrice * quant);
                portfolio.push_back({ticker, quant, openPrice, stocks->date, "SOLD"});
                cout << "Stock " << ticker << " SOLD " << quant << endl;
            }
        }

        // Setting strategy and storing in "AbstractStrategy *strat"
        void setStrategy(int type) {
            if (strat) {
                if (stocks) {
                    stocks->detach(strat);
                }
                delete strat;
                strat = nullptr;
            }
            switch(type) {
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
            std::cout << "[Robot] Strategy set to " << strategyName << std::endl;
        }

        // Using "AbstractStrategy *strat" to call its respective algorithm
        // Stock quantity "1" just for simplicity
        void executeStrat() {
            if (!strat || !stocks) {
                std::cout << "[Robot] Strategy or stocks not set!" << std::endl;
                return;
            }

            std::cout << "\n[Robot] Executing strategy '" << strategyName << "' for date " << stocks->date << std::endl;

            vector<string> temp = strat->getStock(0);
            if(!temp.empty()) {
                for(int i = 0; i < temp.size(); i++) {
                    buy(temp[i], 1);
                }
            }
            temp = strat->getStock(1);
            if(!temp.empty()) {
                for(int i = 0; i < temp.size(); i++) {
                    int quantity = wallet[temp[i]];
                    if(wallet[temp[i]] && (quantity > 0)) {
                        sell(temp[i], quantity);
                    }
                }
            }
            updateStockBalance();
        }

        /*
        // Simulating day to day trading based on unique days in our data
        void updateDate() {
            if (indexDate < 0 || indexDate >= dates.size() - 1) {
                cout << "Out of bounds" << endl; // or throw an exception
            } else {
                date = dates[indexDate - 1];
                vector<StockData> stonk = dateGroups[date];
                indexDate -= 1;
            }
        }
        */

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

            for (const auto& p : wallet) {
                if (p.second > 0) {
                    cout << left
                        << setw(10) << p.first
                        << setw(10) << p.second
                        << endl;
                }
            }

            stringstream logStream;
            logStream << "\n";
            logStream << "Date: " << stocks->date << std::endl;
            logStream << "=== BALANCE ===" << std::endl;
            logStream << "Balance: " << balance << std::endl;
            logStream << "$ in Stocks: " << stockBalance << std::endl;
            logStream << "Total Profit: " << ((balance + stockBalance) - 100000) << std::endl;
            logStream << "=== CURRENT HOLDINGS ===" << std::endl;
            
            // Note: The formatting manipulators (setw, left) work directly with stringstream
            logStream << std::left << std::setw(10) << "Ticker"
                    << std::setw(10) << "Quantity" << std::endl;
            logStream << std::string(20, '-') << std::endl;

            for (const auto& p : wallet) {
                if (p.second > 0) {
                    logStream << std::left
                            << std::setw(15) << p.first
                            << std::setw(15) << p.second
                            << std::endl;
                }
            }

            // 3. Get the resulting string and pass it to appendLog
            string logString = logStream.str();
            logSummary = logString;
        }

};


#endif