#pragma once
#include <string>
#include "raylib.h"

/**
 * @brief Classe base para qualquer propriedade/ativo do jogo.
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

public:
    Property(const std::string& name, const std::string& desc, float price, float rent, Color color);
    virtual ~Property() = default;

    // Getters
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    float getPrice() const { return currentPrice; }
    float getRent() const { return currentRent; }
    int getOwner() const { return owner; }
    Color getColor() const { return color; }
    bool isMortgaged() const { return mortgaged; }

    // Setters
    void setOwner(int newOwner) { owner = newOwner; }
    void setMortgaged(bool status) { mortgaged = status; }

    // Métodos de preço
    void updatePrice(float percentage);
    void updateRent(float percentage);
    void resetToBasePrice();
    
    // Virtual methods para diferentes tipos de propriedades
    virtual std::string getType() const = 0;
    virtual float getPriceVariationRange() const = 0;
    
    // Métodos de negócio
    bool canBeBought(int playerMoney) const;
    void buy(int playerId);
};
