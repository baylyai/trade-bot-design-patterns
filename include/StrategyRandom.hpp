#ifndef STRATEGYRANDOM_H
#define STRATEGYRANDOM_H
#include <string>
#include "AbstractStrategy.hpp"

using namespace std;

class StrategyRandom : public AbstractStrategy {
    public:
        StrategyRandom() {
         
        }
        ~StrategyRandom() {}
        vector<string> getStock() {
            return buyTickers;
        }
        void setStocks(vector<string> tickers) {
            buyTickers = tickers;
        }
};

#endif