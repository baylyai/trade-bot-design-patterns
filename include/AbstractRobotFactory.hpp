#ifndef ABSTRACTROBOTFACTORY_H
#define ABSTRACTROBOTFACTORY_H
#include "Robot.hpp"

using namespace std;

class AbstractRobotFactory {
    public:
        virtual Robot *createRobot() = 0;
};


#endif