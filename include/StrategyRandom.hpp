#ifndef STRATEGYRANDOM_H
#define STRATEGYRANDOM_H

#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "AbstractStrategy.hpp"

using namespace std;

class StrategyRandom : public AbstractStrategy {
private:
    vector<string> allTickers;

public:
    StrategyRandom() {
        srand(static_cast<unsigned>(time(nullptr)));
    }

    ~StrategyRandom() {}
    void setStocks(vector<string> tickers, int type) {
        allTickers = tickers;
    }

    vector<string> getStock(int type) {
        vector<string> result;

        if (allTickers.empty()) {
            return result;
        }
        int randomIndex = rand() % allTickers.size();
        string chosenTicker = allTickers[randomIndex];

        result.push_back(chosenTicker);
        return result;
    }

};

#endif
