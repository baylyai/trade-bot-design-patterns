#ifndef ROBOT_H
#define ROBOT_H
#include <unordered_map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include "AbstractRobot.hpp"
#include "ReadCSV.hpp"

using namespace std;

class Robot : public AbstractRobot {
    public:
        Robot() {
            // lets say the current date is:
            date = "2025-06-23";

            balance = 5000.00;
            stockBalance = 0;
            
            string filename = "World-Stock-Prices-Dataset.csv";

            // # of rows to read in csv up to 300,000
            int records = 10000;

            // read csv
            vector<StockData> stockDataList = readCSVToStockData(filename, records);

            // hash map: ticker -> date -> open price
            marketData = convertToMarketData(stockDataList, true);

            // unique dates for incremeting later
            dates = getUniqueDates(stockDataList);

            for (int i = 0; i < dates.size(); i++) {
                if (dates[i] == date) {
                    indexDate = i;
                    break;
                }
            }


            // hash map when enter ticker returns all stock instances
            //unordered_map<string, vector<StockData>> tickerGroups = groupStockDataByTicker(stockDataList);
            //displayTickerSummary(tickerGroups, "AMZN");

            // hash map when enter date returns all tickers that are open
            unordered_map<string, vector<StockData>> dateGroups = groupStockDataByDate(stockDataList);
            displayDateSummary(dateGroups, date);
            
        }

        void buy(string ticker, uint quant) {
            // check price
            double openPrice = marketData[ticker][date];

            // check can afford
            if((quant * openPrice) < balance) {
                balance -= (openPrice * quant);
                stockBalance += (openPrice * quant);
                // keeping track of history
                portfolio.push_back({ticker, quant, openPrice, date, "BOUGHT"});
                if(!wallet[ticker]) {
                    wallet[ticker] = quant;
                } else {
                    wallet[ticker] += quant;
                }
            }

        }

        void sell(string ticker, uint quant) {
            double openPrice = marketData[ticker][date];
            // check has stock
            if(wallet[ticker] >= quant) {
                wallet[ticker] -= quant;
                balance += (openPrice * quant);
                stockBalance -= (openPrice * quant);
                portfolio.push_back({ticker, quant, openPrice, date, "SOLD"});
            }
        }

        // Simulating day to day trading based on unique days in our data
        void updateDate() {
            if (indexDate < 0 || indexDate >= dates.size() - 1) {
                cout << "Out of bounds" << endl; // or throw an exception
            } else {
                date = dates[indexDate - 1];
            }
        }

        void summary() {
            cout << "Date: " << date << endl;
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