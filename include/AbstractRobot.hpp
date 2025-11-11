#ifndef ABSTRACTROBOT_H
#define ABSTRACTROBOT_H
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class AbstractRobot {
    public:
        AbstractRobot() {}
        virtual void buy(string ticker, uint quantity) = 0;
        virtual void sell(string ticker, uint quantity) = 0;
        virtual void summary() = 0;
    protected:
        double balance;
        double stockBalance;
        string date;
        unordered_map<string, int> wallet;
        vector<tuple<string, int, double, string, string>> portfolio;
        unordered_map<string, unordered_map<string, double>> marketData;
};


#endif