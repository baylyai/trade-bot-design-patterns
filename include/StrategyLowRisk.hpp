#ifndef STRATEGYLOWRISK_H
#define STRATEGYLOWRISK_H
#include <string>
#include "AbstractStrategy.hpp"

using namespace std;

class StrategyLowRisk : public AbstractStrategy {
    public:
        StrategyLowRisk() {
       
        }
        ~StrategyLowRisk() {}
        vector<string> getStock(int type) {
            return buyTickers;
        }
        void setStocks(vector<string> tickers, int type) {
            buyTickers = tickers;
        }
};

#endif