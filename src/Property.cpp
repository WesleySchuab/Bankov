#include "Property.hpp"
#include <cmath>

Property::Property(const std::string& name, const std::string& desc, float price, float rent, Color color, int position)
    : name(name), description(desc), basePrice(price), currentPrice(price), 
      baseRent(rent), currentRent(rent), owner(0), color(color), mortgaged(false), position(position) {}

void Property::updatePrice(float percentage) {
    float variation = currentPrice * (percentage / 100.0f);
    currentPrice += variation;
    
    // Garante que o preço não fique negativo nem muito baixo
    if (currentPrice < basePrice * 0.3f) {
        currentPrice = basePrice * 0.3f;
    }
    
    // Limite máximo de 300% do preço base
    if (currentPrice > basePrice * 3.0f) {
        currentPrice = basePrice * 3.0f;
    }
}

void Property::updateRent(float percentage) {
    float variation = currentRent * (percentage / 100.0f);
    currentRent += variation;
    
    // Garante que o aluguel não fique negativo
    if (currentRent < 1.0f) {
        currentRent = 1.0f;
    }
    
    // Limite máximo de 300% do aluguel base
    if (currentRent > baseRent * 3.0f) {
        currentRent = baseRent * 3.0f;
    }
}

void Property::resetToBasePrice() {
    currentPrice = basePrice;
    currentRent = baseRent;
}

bool Property::canBeBought(int playerMoney) const {
    // Verifica se a propriedade pode ser comprada pelo jogador
    return owner == 0 && playerMoney >= currentPrice && !mortgaged;
}

void Property::buy(int playerId) {
    // Marca a propriedade como comprada pelo jogador indicado
    if (owner == 0) {
        owner = playerId;
    }
}