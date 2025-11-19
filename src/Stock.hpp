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
     * @param position Posição no tabuleiro (0-39).
     */
    Stock(const std::string& ticker, const std::string& company, float price, float dividend, Color color, int position);
    
    /** Retorna o tipo de propriedade ("AÇÃO"). */
    std::string getType() const override { return "AÇÃO"; }
    /** Faixa de variação de preço típica para ações (em %). */
    // Quando a selic muda em 1 ponto, ações tendem a variar ~10%
    float getPriceVariationRange() const override;
    
    /** Retorna o ticker da ação. */
    std::string getTicker() const { return ticker; }
};