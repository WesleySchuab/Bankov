#pragma once
#include <string>
#include "raylib.h"

/**
 * @brief Classe base para qualquer propriedade/ativo do jogo.
 *
 * Guarda preço base/atual, aluguel, dono, cor e estado de hipoteca.
 */
class Property {
protected:
    std::string name;
    std::string description;
    float basePrice;
    float currentPrice;
    float baseRent;
    float currentRent;
    int owner; // 0 = sem dono, 1 = jogador1, 2 = jogador2
    Color color;
    bool mortgaged;
    int position; // Posição no tabuleiro (0-39)

public:
    /**
     * @brief Construtor da propriedade.
     * @param name Nome a ser exibido.
     * @param desc Descrição/legenda curta.
     * @param price Preço base/inicial.
     * @param rent Valor de aluguel/dividendo base.
     * @param color Cor usada na interface.
     * @param position Posição no tabuleiro (0-39).
     */
    Property(const std::string& name, const std::string& desc, float price, float rent, Color color, int position);
    virtual ~Property() = default;

    // Getters
    /** Retorna o nome da propriedade. */
    std::string getName() const { return name; }
    /** Retorna a descrição da propriedade. */
    std::string getDescription() const { return description; }
    /** Retorna o preço atual de mercado da propriedade. */
    float getPrice() const { return currentPrice; }
    /** Retorna o aluguel/dividendo atual da propriedade. */
    float getRent() const { return currentRent; }
    /** Retorna o id do dono (0 = sem dono). */
    int getOwner() const { return owner; }
    /** Retorna a cor associada à propriedade (UI). */
    Color getColor() const { return color; }
    /** Retorna a posição no tabuleiro (0-39). */
    int getPosition() const { return position; }
    /** Indica se a propriedade está hipotecada. */
    bool isMortgaged() const { return mortgaged; }

    // Setters
    /** Define o dono da propriedade. */
    void setOwner(int newOwner) { owner = newOwner; }
    /** Define o estado de hipoteca. */
    void setMortgaged(bool status) { mortgaged = status; }

    // Métodos de preço
    /**
     * @brief Atualiza o preço atual em porcentagem (ex: +10 => +10%).
     * @param percentage Percentual a aplicar sobre o preço atual.
     */
    void updatePrice(float percentage);
    /**
     * @brief Atualiza o aluguel atual em porcentagem.
     * @param percentage Percentual a aplicar sobre o aluguel atual.
     */
    void updateRent(float percentage);
    /** Restaura preço e aluguel para os valores base. */
    void resetToBasePrice();
    
    // Virtual methods para diferentes tipos de propriedades
    /** Retorna o tipo textual da propriedade (ex: "FII", "AÇÃO"). */
    virtual std::string getType() const = 0;
    /** Retorna a faixa de variação típica (em %) para este tipo de ativo. */
    virtual float getPriceVariationRange() const = 0;
    
    // Métodos de negócio
    /**
     * @brief Verifica se a propriedade pode ser comprada por um jogador com given money.
     * @param playerMoney Quantia de dinheiro do jogador.
     * @return true se estiver à venda e o jogador puder pagar.
     */
    bool canBeBought(int playerMoney) const;
    /** Marca a propriedade como comprada pelo jogador indicado. */
    void buy(int playerId);
};