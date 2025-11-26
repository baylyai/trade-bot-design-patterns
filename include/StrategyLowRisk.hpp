#ifndef STRATEGYLOWRISK_H
#define STRATEGYLOWRISK_H
#include <string>
#include "AbstractStrategy.hpp"

using namespace std;

class StrategyLowRisk : public AbstractStrategy {
    public:
        ~StrategyLowRisk() {}
        string pickStock() {
            // need logic here
            // returning DAL as an example
            return "KO";
        }
};

#endif