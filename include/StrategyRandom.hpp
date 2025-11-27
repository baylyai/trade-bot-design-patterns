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
        string pickStock() {
            // need logic here
            // returning DAL as an example
            return "DAL";
        }
};

#endif