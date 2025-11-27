#ifndef READCSV_HPP
#define READCSV_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include "StockData.hpp"
using namespace std;



// Function 1: Read CSV and populate StockData objects
vector<StockData> readCSVToStockData(const string& filename, const int records) {
    vector<StockData> stockDataList;
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return stockDataList;
    }
    
    string line;
    // skip first row (headers)
    getline(file, line); 
    
    // row counter
    int recordCount = 0;
    
    // get specified number of rows
    while (getline(file, line) && recordCount < records) {
        
        // storing the line in ss
        stringstream ss(line);

        // tokenizing
        vector<string> tokens;
        string token;
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        // only grabbing specific attributes
        StockData stock;
        stock.Date = tokens[0].substr(0, tokens[0].find(' '));
        stock.Open = stod(tokens[1]);
        stock.Close = stod(tokens[4]);
        stock.Brand_Name = tokens[6];
        stock.Ticker = tokens[7];
        
        stockDataList.push_back(stock);
        recordCount++;
    }
    
    file.close();
    return stockDataList;
}

#endif