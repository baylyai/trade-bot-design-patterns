#ifndef ROBOTFACTORY_H
#define ROBOTFACTORY_H
#include "AbstractRobotFactory.hpp"

using namespace std;

class RobotFactory : public AbstractRobotFactory {
    Robot* createRobot();
};

Robot* RobotFactory::createRobot() {
    
}


#endif