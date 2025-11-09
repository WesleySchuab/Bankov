#include "Stock.hpp"

Stock::Stock(const std::string& ticker, const std::string& company, float price, float dividend, Color color)
    : Property(company, "Açao " + ticker, price, dividend, color), ticker(ticker) {}
