#ifndef ABSTRACTSTRATEGY_H
#define ABSTRACTSTRATEGY_H
#include <string>
#include "StockData.hpp"

using namespace std;

class AbstractStrategy {
    public:
        virtual ~AbstractStrategy() = default;
        virtual void setStocks(vector<string>, int type) = 0;
        virtual vector<string> getStock(int type) = 0;

    protected:
        //vector<StockData> stockData;
        vector<string> buyTickers;
        vector<string> sellTickers;
};

#endif