#pragma once
#include <string>

// Forward declaration para evitar include circular
class Player;

/**
 * @brief Representa um evento aleatório que pode afetar um jogador ou o mercado.
 */
class Event {
private:
    std::string description;
    float moneyEffect;
    int positionEffect; // movimento de casas (positivo = avança, negativo = volta)
    float marketPercent; // percentual de shift de mercado (ex: +5.0 => +5%)
    std::string sectorTarget; // ex: "commodities", "retail", "banks" ou vazio para todos

public:
    /**
     * @brief Construtor do evento.
     * @param desc Texto descritivo do evento.
     * @param moneyEffect Efeito em dinheiro sobre o jogador (positivo ou negativo).
    * @param marketPercent Percentual de ajuste de mercado (0 = sem ajuste).
    * @param sectorTarget Setor alvo do ajuste (string vazia = todos).
    */
    // Construtor compatível com diferentes chamadas usadas no projeto:
    // - Event(desc, moneyEffect)
    // - Event(desc, moneyEffect, positionEffect)
    // - Event(desc, moneyEffect, positionEffect, marketPercent)
    // - Event(desc, moneyEffect, positionEffect, marketPercent, sectorTarget)
    Event(const std::string& desc, float moneyEffect, int positionEffect = 0, float marketPercent = 0.0f, const std::string& sectorTarget = "");
    
    std::string getDescription() const { return description; }
    float getMoneyEffect() const { return moneyEffect; }
    // positionEffect: se diferente de zero, representa avanço/retrocesso de casas
    int getPositionEffect() const { return positionEffect; }
    float getMarketPercent() const { return marketPercent; }
    std::string getSectorTarget() const { return sectorTarget; }
    
    /**
     * @brief Executa o efeito do evento sobre um jogador (ex: adicionar/remover dinheiro).
     * @param player Jogador afetado.
     */
    void execute(class Player& player);
    
    // Factory methods para eventos comuns
    static Event createRandomEvent();
    static Event createDividendEvent();
    static Event createTaxEvent();
    static Event createMarketUpEvent();
    static Event createMarketDownEvent();
};