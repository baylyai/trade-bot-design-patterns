#ifndef ABSTRACTROBOT_H
#define ABSTRACTROBOT_H
#include <unordered_map>
#include <vector>
#include <string>
#include "StockData.hpp"

using namespace std;

class AbstractRobot {
    public:
        AbstractRobot() {}
        virtual void buy(string ticker, int quantity) = 0;
        virtual void sell(string ticker, int quantity) = 0;
        virtual void summary() = 0;
        //virtual void updateDate() = 0;
        virtual void setStrategy(int type) = 0;
        virtual void executeStrat() = 0;
        virtual void getStockData(StockMarket*) = 0;
        virtual void updateStockBalance() = 0;
        virtual void sellAll() = 0;
        string logSummary;
    protected:
        double balance;
        double stockBalance;
        int indexDate;
        string date;
        vector<string> dates;
        unordered_map<string, int> wallet;
        vector<tuple<string, int, double, string, string>> portfolio;
        unordered_map<string, unordered_map<string, double>> marketData;
};


#endif