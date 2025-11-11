#ifndef ROBOT_H
#define ROBOT_H
#include <unordered_map>
#include <vector>
#include "AbstractRobot.hpp"
#include "ReadCSV.hpp"

using namespace std;

class Robot : public AbstractRobot {
    public:
        Robot() {
            balance = 500.00;
            stockBalance = 0;
            
            string filename = "World-Stock-Prices-Dataset.csv";

            // # of rows to read in csv up to 300,000
            int records = 10000;

            // read csv
            vector<StockData> stockDataList = readCSVToStockData(filename, records);

            // hash map: ticker -> date -> open price
            marketData = convertToMarketData(stockDataList, true);

            // lets say the current date is:
            date = "2025-06-23";

            // hash map when enter ticker returns all stock instances
            //unordered_map<string, vector<StockData>> tickerGroups = groupStockDataByTicker(stockDataList);
            //displayTickerSummary(tickerGroups, "AMZN");

            // hash map when enter date returns all tickers that are open
            //unordered_map<string, vector<StockData>> dateGroups = groupStockDataByDate(stockDataList);
            //displayDateSummary(dateGroups, date);
            
        }

        void buy(string ticker, uint quant) {
            // check price
            double openPrice = marketData[ticker][date];

            // check can afford
            if((quant * openPrice) < balance) {
                balance -= (openPrice * quant);
                stockBalance += openPrice;
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

        void summary() {
            
        }

};


#endif