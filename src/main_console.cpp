#include <iostream>
#include <string>
#include "../include/RobotFactory.hpp"
#include "../include/StockData.hpp"

using namespace std;

int main() {

    // BARE DESIGN
    // Robot will buy stock given current date and specific stock.
    AbstractRobotFactory *fac = new RobotFactory;
    AbstractRobot *robot[3];
    StockMarket Stocks;


    robot[0] = fac->createRobot();
    robot[0]->getStockData(&Stocks);
    
    // Pick strategy
    robot[0]->setStrategy(2);

    // executeStrat will call the algorithm
    // algorithm will return a string for ticker
    // then calls buy()
    robot[0]->summary();
    for(int i = 0; i < 100; i++) {
        Stocks.updateDate();
        robot[0]->executeStrat();
        robot[0]->summary();
    }
    /*
    robot[0]->buy("AMZN", 1);
    robot[0]->buy("RBLX", 2);
    robot[0]->buy("CROX", 5);
    robot[0]->summary();
    Stocks.updateDate();
    robot[0]->buy("DAL", 1);
    Stocks.updateDate();
    robot[0]->buy("DAL", 1);
    Stocks.updateDate();
    robot[0]->buy("DAL", 1);
    robot[0]->summary();
    robot[0]->buy("PTON", 15);
    Stocks.updateDate();
    robot[0]->summary();
    */
    return 0;
}
