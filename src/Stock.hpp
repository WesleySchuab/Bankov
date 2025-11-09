#pragma once
#include "Property.hpp"

class Stock : public Property {
private:
    std::string ticker;

public:
    /**
     * @brief Construtor da ação.
     * @param ticker Código da ação (ex: "PETR4").
     * @param company Nome da empresa.
     * @param price Preço inicial.
     * @param dividend Dividend yield / renda base.
     * @param color Cor usada na interface.
     */
    Stock(const std::string& ticker, const std::string& company, float price, float dividend, Color color);
    
    /** Retorna o tipo de propriedade ("AÇÃO"). */
    std::string getType() const override { return "AÇÃO"; }
    /** Faixa de variação de preço típica para ações (em %). */
    float getPriceVariationRange() const override { return 10.0f; } // ±10% para ações
    
    /** Retorna o ticker da ação. */
    std::string getTicker() const { return ticker; }
};