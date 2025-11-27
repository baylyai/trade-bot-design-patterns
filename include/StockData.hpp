#ifndef STOCKDATA_HPP
#define STOCKDATA_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include "AbstractStrategy.hpp"
//#include "DataManipulation.hpp"
using namespace std;

struct StockData {
    string Date;
    double Open;
    double Close;
    string Brand_Name;
    string Ticker;
};

class StockMarket {
private:
    vector<AbstractStrategy*> observers;
    vector<StockData> Stocks;
public:
    int indexDate;
    string date;
    vector<string> dates;
    unordered_map<string, unordered_map<string, double>> marketData;
    unordered_map<string, vector<StockData>> dateGroups;
    vector<StockData> stockDataList;
    StockMarket() {
        // lets say the current date is:
        date = "2025-06-23";
        
        string filename = "World-Stock-Prices-Dataset.csv";

        // # of rows to read in csv up to 300,000
        int records = 10000;

        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
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
        // read csv
        //vector<StockData> stockDataList = readCSVToStockData(filename, records);

        // hash map: ticker && date -> open price
        marketData = convertToMarketData(stockDataList);

        // unique dates for incremeting later
        dates = getUniqueDates(stockDataList);
        
        for (int i = 0; i < dates.size(); i++) {
            if (dates[i] == date) {
                indexDate = i;
                break;
            }
        }
        // hash map when enter ticker returns all stock instances
        //unordered_map<string, vector<StockData>> tickerGroups = groupStockDataByTicker(stockDataList);
        //displayTickerSummary(tickerGroups, "AMZN");

        // hash map when enter date returns all tickers that are open
        dateGroups = groupStockDataByDate(stockDataList);
        displayDateSummary(dateGroups, date);
        
    }
    void updateDate() {
        date = dates[indexDate - 1];
        Stocks = dateGroups[date];
        indexDate -= 1;
    }
    vector<string> getUniqueTickers(const vector<StockData>& stockDataList) {
        unordered_map<string, bool> tickerMap;
        vector<string> uniqueTickers;
        
        for (const auto& stock : stockDataList) {
            // If ticker not already in map, add it
            if (tickerMap.find(stock.Ticker) == tickerMap.end()) {
                tickerMap[stock.Ticker] = true;
                uniqueTickers.push_back(stock.Ticker);
            }
        }
        
        return uniqueTickers;
    }
    // Function to get unique Dates from StockData objects
    vector<string> getUniqueDates(const vector<StockData>& stockDataList) {
        unordered_map<string, bool> dateMap;
        vector<string> uniqueDates;
        
        for (const auto& stock : stockDataList) {
            // If date not already in map, add it
            if (dateMap.find(stock.Date) == dateMap.end()) {
                dateMap[stock.Date] = true;
                uniqueDates.push_back(stock.Date);
            }
        }
        
        return uniqueDates;
    }

    // Function to convert StockData vector to market data hash map
    unordered_map<string, unordered_map<string, double>> convertToMarketData(const vector<StockData>& stockDataList, bool useOpenPrice = true) {
        unordered_map<string, unordered_map<string, double>> Data;
        
        for (vector<StockData>::const_iterator it = stockDataList.begin(); it != stockDataList.end(); ++it) {
            const StockData& stock = *it;
            double price = useOpenPrice ? stock.Open : stock.Close;
            Data[stock.Ticker][stock.Date] = price;
        }
        
        return Data;
    }

    // Function 2: Group StockData objects by ticker into hash map
    unordered_map<string, vector<StockData>> groupStockDataByTicker(const vector<StockData>& stockDataList) {
        unordered_map<string, vector<StockData>> tickerGroups;
        
        for (const StockData& stock : stockDataList) {
            tickerGroups[stock.Ticker].push_back(stock);
        }
        
        return tickerGroups;
    }

    // Function 3: Group StockData objects by date into hash map
    unordered_map<string, vector<StockData>> groupStockDataByDate(const vector<StockData>& stockDataList) {
        unordered_map<string, vector<StockData>> dateGroups;
        
        for (const StockData& stock : stockDataList) {
            dateGroups[stock.Date].push_back(stock);
        }
        
        return dateGroups;
    }
    void attach(AbstractStrategy* strat) {
        observers.push_back(strat);
    }

    void detach(AbstractStrategy* strat) {
        observers.erase(
            remove(observers.begin(), observers.end(), strat),
            observers.end()
        );
    }

    void notify() {
        for(int i = 0; i < observers.size(); i++) {
            //observers[i]->update(marketData);
        }
    }

    void setMarketData(const vector<StockData>& data) {
        Stocks = data;
        notify();  // Automatically notify observers when data changes
    }

    const vector<StockData>& getMarketData() const {
        return Stocks;
    }
    void displayDateSummary(const unordered_map<string, vector<StockData>>& dateGroups, const string specificDate) {
        
        /*
        // Display summary by date
        cout << "\n=== RECORDS PER DATE ===" << endl;
        
        for (const auto& pair : dateGroups) {
            cout << "Date: " << pair.first << " | Records: " << pair.second.size() << endl;
        }
        */
        // Show all records for a specific date
        cout << "\n=== ALL RECORDS FOR DATE: " << specificDate << " ===" << endl;
        auto find_it = dateGroups.find(specificDate);
        if (find_it != dateGroups.end()) {
            const vector<StockData>& stocks = find_it->second;
            for (const StockData& stock : stocks) {
                cout << "  " << stock.Ticker << " | Open: " << stock.Open << " | Close: " << stock.Close;
                cout << " | Change: " << stock.Close - stock.Open << " | Brand: " << stock.Brand_Name << endl;
            }
        } else {
            cout << "Date '" << specificDate << "' not found in data!" << endl;
        }
    }

    void displayTickerSummary(const unordered_map<string, vector<StockData>>& tickerGroups, const string specificTicker) {
        
        /*
        // Display summary by ticker
        cout << "\n=== RECORDS PER TICKER ===" << endl;
        
        unordered_map<string, vector<StockData>>::const_iterator it;
        for (it = tickerGroups.begin(); it != tickerGroups.end(); ++it) {
            cout << "Ticker: " << it->first << " | Records: " << it->second.size() << endl;
        }
        */
        
        // Show all records for a specific ticker
        cout << "\n=== ALL RECORDS FOR TICKER: " << specificTicker << " ===" << endl;
        unordered_map<string, vector<StockData>>::const_iterator find_it = tickerGroups.find(specificTicker);
        if (find_it != tickerGroups.end()) {
            const vector<StockData>& stocks = find_it->second;
            for (size_t i = 0; i < stocks.size(); ++i) {
                const StockData& stock = stocks[i];
                cout << "  " << stock.Date << " | Open: " << stock.Open << " | Close: " << stock.Close;
                cout << " | Change: " << stock.Close - stock.Open << endl;
            }
        } else {
            cout << "Ticker '" << specificTicker << "' not found in data!" << endl;
        }
    }
};

#endif