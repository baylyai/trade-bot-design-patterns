#include <iostream>
#include <string>
#include "../include/RobotFactory.hpp"

using namespace std;

int main() {

    // BARE DESIGN
    // Robot will buy stock given current date and specific stock.
    AbstractRobotFactory *fac = new RobotFactory;
    AbstractRobot *robot[3];

    robot[0] = fac->createRobot();
    
    // Pick strategy
    robot[0]->setStrategy(0);

    // executeStrat will call the algorithm
    // algorithm will return a string for ticker
    // then calls buy()
    robot[0]->executeStrat();

    robot[0]->buy("AMZN", 1);
    robot[0]->buy("RBLX", 2);
    robot[0]->buy("CROX", 5);
    robot[0]->summary();
    robot[0]->updateDate();
    robot[0]->buy("PTON", 15);
    robot[0]->buy("UBER", 3);
    robot[0]->summary();
    return 0;
}