#include <iostream>
#include "../include/ReadCSV.hpp"

using namespace std;

int main() {
    string filename = "World-Stock-Prices-Dataset.csv";

    // # of rows to read in csv up to 300,000
    int records = 10000;
    unordered_map<string, vector<StockData>> tickerGroups = readAndGroupByTicker(filename, records);
    displayTickerSummary(tickerGroups);
    

    return 0;
}