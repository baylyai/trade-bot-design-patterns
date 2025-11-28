#ifndef ABSTRACTSTRATEGY_H
#define ABSTRACTSTRATEGY_H
#include <string>
#include "StockData.hpp"

using namespace std;

class AbstractStrategy {
    public:
        virtual ~AbstractStrategy() = default;
        virtual void setStocks(vector<string>) = 0;
        virtual vector<string> getStock() = 0;

    protected:
        //vector<StockData> stockData;
        vector<string> buyTickers;
};

#endif