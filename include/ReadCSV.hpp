#ifndef READCSV_HPP
#define READCSV_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "StockData.hpp"

using namespace std;

// split one line of text by commas
vector<string> splitLineByComma(const string &line)
{
    vector<string> pieces;
    string piece;
    stringstream lineStream(line);
    while (getline(lineStream, piece, ',')) {
        pieces.push_back(piece);
    }

    return pieces;
}

// Read CSV file and return up to records row
vector<StockData> readCSVToStockData(const string &filename, int records)
{
    vector<StockData> stockDataList;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return stockDataList;
    }
    int recordCount = 0;
    string line;
    getline(file, line);

    // read each remaining line until record limit
    while (getline(file, line) && recordCount < records) {

        // split line into columns based on commas
        vector<string> columns = splitLineByComma(line);

        if (columns.size() < 8) {
            continue;
        }
        StockData stock;

        string fullDate = columns[0];
        size_t spacePos = fullDate.find(' ');
        if (spacePos != string::npos) {
            stock.Date = fullDate.substr(0, spacePos);
        } else {
            stock.Date = fullDate;
        }

        // string to decimal
        stock.Open  = stod(columns[1]); // Open price
        stock.Close = stod(columns[4]); // Close price

        // Strings for name and ticker
        stock.Brand_Name = columns[6];  // Company/brand name
        stock.Ticker     = columns[7];  // Stock ticker

        // Add stock to list
        stockDataList.push_back(stock);
        recordCount++;
    }

    file.close();
    return stockDataList;
}

#endif
