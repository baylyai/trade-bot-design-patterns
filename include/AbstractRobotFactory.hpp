#ifndef ABSTRACTROBOTFACTORY_HPP
#define ABSTRACTROBOTFACTORY_HPP

#include "AbstractRobot.hpp"

class AbstractRobotFactory
{
public:
    virtual AbstractRobot* createRobot() = 0;
};

#endif
