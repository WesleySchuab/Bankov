#include "FII.hpp"

FII::FII(const std::string& ticker, const std::string& fundName, float price, float dividendYield, Color color, int position)
    : Property(fundName, "FII " + ticker, price, dividendYield, color, position), ticker(ticker) {}