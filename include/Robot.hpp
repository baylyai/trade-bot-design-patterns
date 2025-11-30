#ifndef ROBOT_H
#define ROBOT_H
#include <unordered_map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
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
    public:
        Robot() {
            balance = 100000;
            stockBalance = 0;
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
        void buy(string ticker, uint quant) {
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
            }

        }

        void sell(string ticker, uint quant) {
            double openPrice = stocks->marketData[ticker][stocks->date];
            // check has stock
            if(wallet[ticker] >= quant) {
                wallet[ticker] -= quant;
                balance += (openPrice * quant);
                //stockBalance -= (openPrice * quant);
                portfolio.push_back({ticker, quant, openPrice, stocks->date, "SOLD"});
            }
        }

        // Setting strategy and storing in "AbstractStrategy *strat"
        void setStrategy(int type) {
            if(strat) {
                delete strat;
                strat = nullptr;
            }
            switch(type) {
                case 0:
                    strat = new StrategyRandom();
                    stocks->attach(strat);
                    break;
                case 1:
                    strat = new StrategyLowRisk();
                    stocks->attach(strat);
                    break;
                default:
                    strat = new StrategyBasic();
                    stocks->attach(strat);
            }
        }

        // Using "AbstractStrategy *strat" to call its respective algorithm
        // Stock quantity "1" just for simplicity
        void executeStrat() {
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

            cout << "=== PORTFOLIO ===" << endl;
            cout << left << setw(10) << "Ticker" 
                    << setw(10) << "Quantity" 
                    << setw(12) << "Price" 
                    << setw(12) << "Date" 
                    << setw(10) << "Action" << endl;
            cout << string(54, '-') << endl;
            
            for (vector<tuple<string, int, double, string, string>>::const_iterator it = portfolio.begin(); 
                it != portfolio.end(); ++it) {
                
                string ticker = get<0>(*it);
                int quantity = get<1>(*it);
                double price = get<2>(*it);
                string date = get<3>(*it);
                string action = get<4>(*it);
    
                cout << left 
                        << setw(10) << ticker 
                        << setw(10) << quantity 
                        << setw(12) << fixed << setprecision(2) << price 
                        << setw(12) << date 
                        << setw(10) << action << endl;
            }
        }
};


#endif