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
    robot[0]->buy("AMZN", 1);

    return 0;
}