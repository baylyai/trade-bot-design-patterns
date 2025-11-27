#ifndef ABSTRACTSTRATEGY_H
#define ABSTRACTSTRATEGY_H
#include <string>
#include "StockData.hpp"

using namespace std;

class AbstractStrategy {
    public:
        virtual ~AbstractStrategy() = default;
        virtual string pickStock() = 0;
        /*
        void update(const vector<StockData>& stockData) {
            // Store the latest stock data
            this->stockData = stockData;
        }
        */

    protected:
        //vector<StockData> stockData;
};

#endif