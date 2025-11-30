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
        vector<string> getStock(int type) {
            return buyTickers;
        }
        void setStocks(vector<string> tickers, int type) {
            buyTickers = tickers;
        }
};

#endif