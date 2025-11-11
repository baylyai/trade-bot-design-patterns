#ifndef ROBOTFACTORY_H
#define ROBOTFACTORY_H
#include "AbstractRobotFactory.hpp"

using namespace std;

class RobotFactory : public AbstractRobotFactory {
    public:
        Robot* createRobot();
};

Robot* RobotFactory::createRobot() {
    return new Robot();
}


#endif