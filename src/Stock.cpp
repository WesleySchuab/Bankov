#include "Stock.hpp"
/// Construtor da ação
Stock::Stock(const std::string& ticker, const std::string& company, float price, float dividend, Color color, int position)
    : Property(company, "Ação " + ticker, price, dividend, color, position), ticker(ticker) {}

// Faixa de variação típica para ações (em %)
float Stock::getPriceVariationRange() const {
    return 10.0f; // ±10% para ações
}