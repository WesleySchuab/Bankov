#pragma once
#include "Property.hpp"
#include "Event.hpp"
#include <vector>
#include <memory>

/**
 * @brief Representa o tabuleiro do jogo, contendo propriedades e eventos.
 */
class Board {
private:
    std::vector<std::shared_ptr<Property>> properties;
    int totalPositions;  // inicializado no construtor
    
public:
    /** Construtor: inicializa o tabuleiro e suas propriedades. */
    Board();
    
    // Getters
    const std::vector<std::shared_ptr<Property>>& getProperties() const { return properties; }
    /** Retorna a propriedade (se houver) na posição do tabuleiro. */
    std::shared_ptr<Property> getPropertyAtPosition(int position) const;
    /** Indica se a posição é uma casa de evento. */
    bool isEventPosition(int position) const;
    /** Retorna um evento aleatório da lista de eventos. */
    Event getRandomEvent() const;
    int getTotalPositions() const { return totalPositions; }
    
    // Métodos de mercado
    /** Atualiza preços de mercado (pode ser noop dependendo da estratégia). */
    // (removido) updateMarketPrices was unused and has been removed to reduce confusion.
    /** Restaura preços ao valor base. */
    void resetMarketPrices();
    /**
     * @brief Aplica um shift percentual nos preços diferenciando ativos de alto DY e os demais.
     * @param percentHighDY Percentual aplicado a ativos de alto DY.
     * @param percentOthers Percentual aplicado aos demais ativos.
     */
    void applyMarketShift(float percentHighDY, float percentOthers);
    /**
     * @brief Aplica um shift percentual a um setor específico (ex: "commodities").
     */
    void applyMarketShiftForSector(const std::string& sector, float percent);
    
    // Configuração do tabuleiro
    /** Popula e configura as propriedades e posições do tabuleiro. */
    void initializeProperties();
};