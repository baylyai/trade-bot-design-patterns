#ifndef ROBOTFACTORY_HPP
#define ROBOTFACTORY_HPP

#include "AbstractRobotFactory.hpp"
#include "Robot.hpp"

class RobotFactory : public AbstractRobotFactory
{
public:
    AbstractRobot* createRobot() override {
        return new Robot();
    }
};

#endif
