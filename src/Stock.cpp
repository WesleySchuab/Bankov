#include "Stock.hpp"
/// Construtor da ação
Stock::Stock(const std::string& ticker, const std::string& company, float price, float dividend, Color color, int position)
    : Property(company, "Ação " + ticker, price, dividend, color, position), ticker(ticker) {}