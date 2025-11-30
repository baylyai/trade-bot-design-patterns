#ifndef TRADELOGGER_HPP
#define TRADELOGGER_HPP

#include <functional>
#include <string>

class TradeLogger {
public:
    static void setLogger(const std::function<void(const std::string&)>& fn);
    static void log(const std::string& msg);

private:
    static std::function<void(const std::string&)> logger_;
};

#endif
