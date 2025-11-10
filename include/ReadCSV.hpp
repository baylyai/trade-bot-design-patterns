#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

struct StockData {
    string Date;
    double Open;
    double Close;
    string Brand_Name;
    string Ticker;
};

unordered_map<string, vector<StockData>> readAndGroupByTicker(const string& filename, const int records) {

    // hash map
    unordered_map<string, vector<StockData>> tickerGroups;
    cout << records << " " << filename << endl;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return tickerGroups;
    }
    
    string line;
    // skip first row(headers)
    getline(file, line); 
    
    // row counter
    int recordCount = 0;
    
    // get 2000 row indices
    while (getline(file, line) && recordCount < records) {

        // error handling if row is empty
        if (line.empty()) continue;
        
        // storing the line in ss
        stringstream ss(line);

        // tokenizing
        vector<string> tokens;
        string token;
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // 13 total attributes
        // only need up to attribute 8
        // header indices
        // Date(0), Open(1), Close(4), Brand_Name(6), Ticker(7)
        if (tokens.size() < 8) continue;
        

        StockData stock;
        stock.Date = tokens[0].substr(0, tokens[0].find(' '));
        stock.Open = stod(tokens[1]);
        stock.Close = stod(tokens[4]);
        stock.Brand_Name = tokens[6];
        stock.Ticker = tokens[7];
        
        tickerGroups[stock.Ticker].push_back(stock);
        recordCount++;
    }
    
    file.close();
    return tickerGroups;
}

void displayTickerSummary(const unordered_map<string, vector<StockData>>& tickerGroups) {
    
    const string& specificTicker = "AMZN";
    // Display summary by ticker
    cout << "\n=== RECORDS PER TICKER ===" << endl;
    for (const pair<string, vector<StockData>>& group : tickerGroups) {
        cout << "Ticker: " << group.first << " | Records: " << group.second.size() << endl;
    }
    
    // Show all records for a specific ticker
    cout << "\n=== ALL RECORDS FOR TICKER: " << specificTicker << " ===" << endl;
    if (tickerGroups.find(specificTicker) != tickerGroups.end()) {
        for (const StockData& stock : tickerGroups.at(specificTicker)) {
            cout << "  " << stock.Date << " | Open: " << stock.Open << " | Close: " << stock.Close;
            cout << " | Change: " << stock.Close - stock.Open << endl;
        }
    } else {
        cout << "Ticker '" << specificTicker << "' not found in data!" << endl;
    }
}
