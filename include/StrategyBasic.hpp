#ifndef STRATEGYBASIC_H
#define STRATEGYBASIC_H
#include <string>
#include "AbstractStrategy.hpp"

using namespace std;

class StrategyBasic : public AbstractStrategy {
    public:
        ~StrategyBasic() {}
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