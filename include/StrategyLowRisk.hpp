#ifndef STRATEGYLOWRISK_H
#define STRATEGYLOWRISK_H
#include <string>
#include "AbstractStrategy.hpp"

using namespace std;

class StrategyLowRisk : public AbstractStrategy {
    public:
        ~StrategyLowRisk() {}
        vector<string> getStock(int type) {
            switch(type) {
                case 0:
                    return buyTickers;
                default:
                    return sellTickers;
            }
        }
        void setStocks(vector<string> tickers, int type) {
            switch(type) {
                case 0:
                    buyTickers = tickers;
                    break;
                default:
                    sellTickers = tickers;
            }
        }
};

#endif