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
        void buy(string ticker, uint quant) {
            // check price
            double openPrice = stocks->marketData[ticker][stocks->date];
            cout << "\nOPEN PRICE HERE \n" << openPrice;

            // check can afford
            if((quant * openPrice) < balance) {
                balance -= (openPrice * quant);
                stockBalance += (openPrice * quant);
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
            double openPrice = stocks->marketData[ticker][date];
            // check has stock
            if(wallet[ticker] >= quant) {
                wallet[ticker] -= quant;
                balance += (openPrice * quant);
                stockBalance -= (openPrice * quant);
                portfolio.push_back({ticker, quant, openPrice, date, "SOLD"});
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
                    break;
                case 1:
                    strat = new StrategyLowRisk();
                    break;
                default:
                    strat = new StrategyBasic();
            }
        }

        // Using "AbstractStrategy *strat" to call its respective algorithm
        // Stock quantity "1" just for simplicity
        void executeStrat() {
            buy(strat->pickStock(), 1);
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