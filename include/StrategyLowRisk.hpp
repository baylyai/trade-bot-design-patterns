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
        string pickStock() {
            // need logic here
            // returning KO as an example
            return "KO";
        }
};

#endif