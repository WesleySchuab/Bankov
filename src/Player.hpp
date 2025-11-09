#pragma once
#include "Property.hpp"
#include <vector>
#include <memory>

/**
 * @brief Representa um jogador do jogo.
 */
class Player {
private:
    int id;
    std::string name;
    Color color;
    float money;
    int position;
    int lapsCompleted; // contador de voltas completas
    std::vector<std::shared_ptr<Property>> properties;

public:
    /**
     * @brief Construtor do jogador.
     * @param id Identificador do jogador (1, 2, ...).
     * @param name Nome exibido.
     * @param color Cor usada na UI.
     * @param initialMoney Dinheiro inicial.
     */
    //** Construtor do jogador. */
    Player(int id, const std::string& name, Color color, float initialMoney);
    
    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }// Nome exibido do jogador
    Color getColor() const { return color; }
    float getMoney() const { return money; }
    int getPosition() const { return position; }
    // Retorna lista de propriedades do jogador
    const std::vector<std::shared_ptr<Property>>& getProperties() const { return properties; }
    
    // Métodos do jogo
    /** Move o jogador em 'steps' casas no tabuleiro (faz wrap). */
    void move(int steps);
    /** Define a posição do jogador (usar para zerar posições). */
    void setPosition(int pos);
    /** Adiciona (ou subtrai) dinheiro do jogador. */
    void addMoney(float amount);
    /** Incrementa o contador de voltas completas do jogador. */
    void incrementLaps();
    /** Retorna quantas voltas completas o jogador já realizou. */
    int getLapsCompleted() const;
    /** Verifica se o jogador pode pagar um valor. */
    bool canAfford(float amount) const;
    /** Executa a compra de uma propriedade pelo jogador. */
    void buyProperty(std::shared_ptr<Property> property);
    /** Vende uma propriedade do jogador (se possuir). */
    void sellProperty(std::shared_ptr<Property> property);
    /** Paga aluguel/dividendo relativo a uma propriedade de outro jogador. */
    void payRent(std::shared_ptr<Property> property);
    /** Calcula ativos totais (dinheiro + valor das propriedades). */
    float calculateTotalAssets() const;
    
    // Reset para novo jogo
    /** Reinicia estado do jogador para um novo jogo. */
    void reset();
};