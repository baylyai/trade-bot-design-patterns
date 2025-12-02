#ifndef STOCKDATA_HPP
#define STOCKDATA_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include "AbstractStrategy.hpp"

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
    int elapsedDays;
    string date;
    vector<string> dates;
    vector<string> allTickers;
    unordered_map<string, unordered_map<string, double>> marketData;
    unordered_map<string, vector<StockData>> dateGroups;
    unordered_map<string, vector<double>> stockPrices;
    vector<StockData> stockDataList;
    StockMarket() {
        // lets say the current date is:
        date = "2020-01-02";
        
        string filename = "World-Stock-Prices-Dataset.csv";

        // # of rows to read in csv up to 300,000
        int records = 300000;
        elapsedDays = 0;

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
                cout << "\nINDEX DATE \n" << indexDate;
                break;
            }
        }
        // hash map when enter ticker returns all stock instances
        //unordered_map<string, vector<StockData>> tickerGroups = groupStockDataByTicker(stockDataList);
        //displayTickerSummary(tickerGroups, "AMZN");

        // hash map when enter date returns all tickers that are open
        allTickers = getUniqueTickers(stockDataList);
        for(const string& ticker : allTickers) {
            stockPrices[ticker] = {};
        }
        for(const string& ticker : allTickers) {
            if(marketData[ticker][date])
                stockPrices[ticker].push_back(marketData[ticker][date]);
        }

        dateGroups = groupStockDataByDate(stockDataList);
        displayDateSummary(dateGroups, date);
    }
    void updateDate() {
        date = dates[indexDate - 1];
        Stocks = dateGroups[date];
        checkStocks();
        sellStocks();
        indexDate -= 1;
        elapsedDays += 1;
    }
    void attach(AbstractStrategy* strat) {
        observers.push_back(strat);
    }
    

    void detach(AbstractStrategy* strat) {
        observers.erase(remove(observers.begin(), observers.end(), strat), observers.end());
    }

    void notify(vector<string> tickers, int type) {
        for(int i = 0; i < observers.size(); i++) {
            observers[i]->setStocks(tickers, type);
        }
    }
    
    void checkStocks() {
        vector<string> buyStocks;
        const int BUY_LIMIT = 4;
        const int LOOKBACK_DAYS = 5; // The period to check for the recent lowest price

        for(const string& ticker : allTickers) {

            if (buyStocks.size() >= BUY_LIMIT) {
                break; 
            }
            
            if(!marketData[ticker].count(date)) {
                continue; // Skip if no data for today
            }
            double currentPrice = marketData[ticker][date];

            // Ensure the full price history vector (stockPrices[ticker]) has enough data
            if (stockPrices[ticker].empty()) {
                stockPrices[ticker].push_back(currentPrice);
                continue; // Need more history before we can make a decision
            }
            
            double yesterdayPrice = stockPrices[ticker].back();
            
            // Always update the full history vector with the current price
            stockPrices[ticker].push_back(currentPrice); 
            
            // Condition A: Price Increased
            if (currentPrice > yesterdayPrice) {
                
                // Limit the check to the last LOOKBACK_DAYS prices
                int start_index = max(0, (int)stockPrices[ticker].size() - (LOOKBACK_DAYS + 1));
                
                // Find the minimum price in the lookback window (excluding today's price)
                double minPrice = currentPrice; 
                for (size_t i = start_index; i < stockPrices[ticker].size() - 1; ++i) {
                    minPrice = min(minPrice, stockPrices[ticker][i]);
                }

                if (yesterdayPrice <= minPrice) { 
                    buyStocks.push_back(ticker);
                }
            }
        }
        
        if(!buyStocks.empty()) {
            notify(buyStocks, 0);
        } 
    }
    
    void sellStocks() {
        vector<string> sellStocks;
        const int LOOKBACK_DAYS = 5; // The period to check for the recent highest price
        
        for(const string& ticker : allTickers) { // Check all tickers for sell signals
    
            if(!marketData[ticker].count(date)) {
                continue; // Skip if no data for today
            }
            
            double currentPrice = marketData[ticker][date];
            
            // Ensure the full price history vector has enough data
            if (stockPrices[ticker].size() < 2) {
                continue; // Need at least 2 prices for comparison
            }
            
            double yesterdayPrice = stockPrices[ticker][stockPrices[ticker].size() - 2];
                        
            // Condition: Price Decreased (inverse of buy logic)
            if (currentPrice < yesterdayPrice) {
                // Limit the check to the last LOOKBACK_DAYS prices
                int start_index = max(0, (int)stockPrices[ticker].size() - (LOOKBACK_DAYS + 1));
                
                // Find the maximum price in the lookback window (excluding today's price)
                double maxPrice = currentPrice;
                for (size_t i = start_index; i < stockPrices[ticker].size() - 1; ++i) {
                    maxPrice = max(maxPrice, stockPrices[ticker][i]);
                }
                
                // Add to sell list if yesterday's price was at/near the recent peak and today it dropped
                if (yesterdayPrice >= maxPrice) {
                    sellStocks.push_back(ticker);
                }
            }
        }
        
        if(!sellStocks.empty()) {
            notify(sellStocks, 1);
        }
    }

    const vector<StockData>& getMarketData() const {
        return Stocks;
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