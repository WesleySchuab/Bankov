#include "Event.hpp"
#include "Player.hpp"
#include "raylib.h" // GetRandomValue
#include <random>

Event::Event(const std::string& desc, float moneyEffect, int positionEffect, float marketPercent, const std::string& sectorTarget)
    : description(desc), moneyEffect(moneyEffect), positionEffect(positionEffect), marketPercent(marketPercent), sectorTarget(sectorTarget) {}

void Event::execute(Player& player) {
    // Aplica efeito monetário
    player.addMoney(moneyEffect);
    // Aplica movimento se houver (retrocompatibilidade)
    if (positionEffect != 0) {
        player.move(positionEffect);
    }
}

Event Event::createRandomEvent() {
    std::vector<Event> possibleEvents = {
        Event("Dividendos! Receba $100", 100.0f, 0),
        Event("Imposto de Renda! Pague $50", -50.0f, 0),
        Event("Fundo desvalorizou! Pague $30", -30.0f, 0),
        Event("Acao valorizou! Receba $80", 80.0f, 0),
        Event("Bolsa em alta! Avance 2 casas", 0.0f, 2),
        Event("Investidores estrangeiros estão investindo no país! Bolsa sobe 40%", 0.0f, 0, 40.0f),
        Event("Notícia muito ruim! Mercado -50%", 0.0f, 0, -50.0f),
        Event("Boa notícia! Mercado +10%", 0.0f, 0, 10.0f),
        Event("COPOM corta Selic! Ativos +15%", 0.0f, 0, 15.0f, "all"),
        Event("Crise do minério: Commodities -30%", 0.0f, 0, -30.0f, "commodities"),
        Event("Alta do varejo: Varejo +12%", 0.0f, 0, 12.0f, "retail"),
        Event("Problema nos bancos: Bancos -20%", 0.0f, 0, -20.0f, "banks"),
        Event("Campanha promocional no varejo: Varejo +8%", 0.0f, 0, 8.0f, "retail"),
        Event("Relatorio positivo de banco: Bancos +6%", 0.0f, 0, 6.0f, "banks"),
        Event("Quebra de mineradora: Commodities -18%", 0.0f, 0, -18.0f, "commodities"),
        Event("FII pagou dividendos! Receba $60", 60.0f, 0),
        Event("Taxa de administracao! Pague $40", -40.0f, 0)
    };

    int randomIndex = GetRandomValue(0, (int)possibleEvents.size() - 1);
    return possibleEvents[randomIndex];
}

Event Event::createDividendEvent() {
    return Event("💸 Dividendos recebidos!", 80.0f);
}

Event Event::createTaxEvent() {
    return Event("📊 Impostos a pagar!", -60.0f);
}

Event Event::createMarketUpEvent() {
    return Event("🚀 Mercado em alta!", 0.0f, 4.0f);
}

Event Event::createMarketDownEvent() {
    return Event("📉 Mercado em baixa!", 0.0f, -4.0f);
}