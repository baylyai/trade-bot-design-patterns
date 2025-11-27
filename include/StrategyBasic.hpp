#ifndef STRATEGYBASIC_H
#define STRATEGYBASIC_H
#include <string>
#include "AbstractStrategy.hpp"

using namespace std;

class StrategyBasic : public AbstractStrategy {
    public:
        ~StrategyBasic() {}
        string pickStock() {
            // need logic here
            
            // returning TM as an example
            return "TM";
        }
};

#endif