#pragma once
#include "Property.hpp"
#include <string>

/**
 * @brief Representa um fundo imobiliário (FII) no tabuleiro.
 */
class FII : public Property {
private:
    std::string ticker;

public:
    /**
     * @brief Construtor do FII.
     * @param ticker Código do fundo (ex: "HGLG11").
     * @param fundName Nome do fundo.
     * @param price Preço inicial.
     * @param dividendYield Rendimento/dividendo base.
     * @param color Cor usada na interface.
     */
    FII(const std::string& ticker, const std::string& fundName, float price, float dividendYield, Color color);
    
    /** Retorna o tipo de propriedade ("FII"). */
    std::string getType() const override { return "FII"; }
    /** Faixa de variação de preço típica para FIIs (em %). */
    float getPriceVariationRange() const override { return 7.0f; } // ±7% para FIIs
    
    /** Retorna o ticker do FII. */
    std::string getTicker() const { return ticker; }
};