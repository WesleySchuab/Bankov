#include "FII.hpp"

FII::FII(const std::string& ticker, const std::string& fundName, float price, float dividendYield, Color color, int position)
    : Property(fundName, "FII " + ticker, price, dividendYield, color, position), ticker(ticker) {}

// Faixa de variação típica para FIIs (em %)
float FII::getPriceVariationRange() const {
    return 7.0f; // ±7% para FIIs
}