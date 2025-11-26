#ifndef ABSTRACTSTRATEGY_H
#define ABSTRACTSTRATEGY_H
#include <string>

using namespace std;

class AbstractStrategy {
    public:
        virtual string pickStock() = 0;
        virtual ~AbstractStrategy() = default;
};

#endif