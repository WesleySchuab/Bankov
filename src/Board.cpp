#include "Board.hpp"
#include "Stock.hpp"
#include "FII.hpp"
#include <random>
#include <algorithm>
#include <cctype>

Board::Board() : totalPositions(40) {  
    initializeProperties();
}

void Board::initializeProperties() {
    properties.clear();
    // Adiciona algumas ações e FIIs ao tabuleiro
    properties.push_back(std::make_shared<Stock>(
        "BBAS3", "Banco do Brasil", 60.0f, 7.0f, (Color){52, 152, 219, 255}
    ));
    
    properties.push_back(std::make_shared<Stock>(
        "PETR4", "Petrobras", 60.0f, 6.0f, (Color){39, 174, 96, 255}
    ));
    
    properties.push_back(std::make_shared<Stock>(
        "VALE3", "Vale S.A.", 100.0f, 6.0f, (Color){142, 68, 173, 255}
    ));
    
    properties.push_back(std::make_shared<FII>(
        "HGLG11", "CSHG Logística", 140.0f, 10.0f, (Color){230, 126, 34, 255}
    ));
    
    properties.push_back(std::make_shared<FII>(
        "XPML11", "XP Malls", 160.0f, 12.0f, (Color){22, 160, 133, 255}
    ));
    
    properties.push_back(std::make_shared<FII>(
        "HGRU11", "CSHG Renda Urbana", 180.0f, 14.0f, (Color){155, 89, 182, 255}
    ));
    
    // Adicionar mais ações para aumentar o número de ativos
    properties.push_back(std::make_shared<Stock>("ITUB4", "Itaú Unibanco", 120.0f, 7.0f, (Color){46, 204, 113, 255}));
    properties.push_back(std::make_shared<Stock>("ABEV3", "Ambev", 80.0f, 3.0f, (Color){241, 196, 15, 255}));
    properties.push_back(std::make_shared<Stock>("BBDC4", "Bradesco", 110.0f, 6.5f, (Color){231, 76, 60, 255}));
    properties.push_back(std::make_shared<Stock>("PETR3", "Petrobras PN", 70.0f, 6.0f, (Color){26, 188, 156, 255}));
    properties.push_back(std::make_shared<Stock>("MGLU3", "Magazine Luiza", 95.0f, 3.5f, (Color){52, 73, 94, 255}));
    properties.push_back(std::make_shared<Stock>("GGBR4", "Gerdau PN", 85.0f, 5.5f, (Color){117, 80, 123, 255}));
    properties.push_back(std::make_shared<Stock>("NATU3", "Natura", 90.0f, 3.8f, (Color){39, 174, 96, 255}));
    properties.push_back(std::make_shared<Stock>("CSAN3", "Cosan", 75.0f, 5.5f, (Color){41, 128, 185, 255}));
    properties.push_back(std::make_shared<Stock>("CMIN3", "CSN Mineração", 65.0f, 6.0f, (Color){142, 68, 173, 255}));
    properties.push_back(std::make_shared<Stock>("LREN3", "Lojas Renner", 105.0f, 4.0f, (Color){52, 152, 219, 255}));
    properties.push_back(std::make_shared<Stock>("RENT3", "Localiza/Movida", 88.0f, 3.6f, (Color){142, 68, 173, 255}));
    properties.push_back(std::make_shared<Stock>("BRFS3", "BRF", 72.0f, 3.2f, (Color){241, 196, 15, 255}));
    properties.push_back(std::make_shared<Stock>("AZUL4", "Azul Linhas Aereas", 68.0f, 2.9f, (Color){231, 76, 60, 255}));
    properties.push_back(std::make_shared<Stock>("WEGE3", "Weg", 140.0f, 5.0f, (Color){26, 188, 156, 255}));
    
    // Adicionar 5 FIIs adicionais
    properties.push_back(std::make_shared<FII>("KNRI11", "Kinea Renda Imobiliaria", 130.0f, 10.0f, (Color){149, 165, 166, 255}));
    properties.push_back(std::make_shared<FII>("VISC11", "Vinci Shopping Centers", 150.0f, 11.0f, (Color){211, 84, 0, 255}));
    properties.push_back(std::make_shared<FII>("MXRF11", "Maxi Renda", 110.0f, 9.0f, (Color){44, 62, 80, 255}));
    properties.push_back(std::make_shared<FII>("HABT11", "Habitasul FII", 95.0f, 9.0f, (Color){22, 160, 133, 255}));
    properties.push_back(std::make_shared<FII>("RBRP11", "RBR Properties", 120.0f, 10.0f, (Color){39, 174, 96, 255}));
    
    // Distribuir propriedades aleatoriamente pelas posições do tabuleiro
    propertyPositions.clear();
    int slots = totalPositions;
    int nProps = (int)properties.size();

    // Criar lista de posições candidatas (0 é preferencialmente reservada para evento)
    std::vector<int> posCandidates;
    for (int p = 0; p < slots; ++p) {
        // Evita usar a posição 0 aqui para garantir que possamos colocar um evento nela
        if (p == 0) continue;
        posCandidates.push_back(p);
    }

    // Se houver mais propriedades do que posições disponíveis (exceto 0), permita usar 0
    if ((int)posCandidates.size() < nProps) {
        posCandidates.clear();
        for (int p = 0; p < slots; ++p) posCandidates.push_back(p);
    }

    std::random_device rd2;// semente diferente para esta aleatorização
    std::mt19937 g2(rd2()); // inicializa o gerador de números aleatórios
    std::shuffle(posCandidates.begin(), posCandidates.end(), g2);// embaralha as posições candidatas

    for (int i = 0; i < nProps && i < (int)posCandidates.size(); ++i) {
        propertyPositions.push_back(posCandidates[i]);
    }

    // Fallback: se por algum motivo ainda não temos posições suficientes, preencha sequencialmente
    if ((int)propertyPositions.size() < nProps) {
        for (int p = 0; p < slots && (int)propertyPositions.size() < nProps; ++p) {
            if (std::find(propertyPositions.begin(), propertyPositions.end(), p) == propertyPositions.end())
                propertyPositions.push_back(p);
        }
    }
    // definir posições de eventos nas posições restantes escolhendo aleatoriamente
    // Mas evitar que fiquem muito juntas: garantir uma distancia minima
    eventPositions.clear();
    std::vector<int> candidates;
    for (int pos = 0; pos < slots; pos++) {
        if (std::find(propertyPositions.begin(), propertyPositions.end(), pos) == propertyPositions.end()) {
            candidates.push_back(pos);
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(candidates.begin(), candidates.end(), g);
    int needed = std::min((int)candidates.size(), 10); // agora queremos até 10 casas de evento
    int minDist = 2; // distancia minima entre eventos
    for (int idx = 0; idx < (int)candidates.size() && (int)eventPositions.size() < needed; ++idx) {
        int pos = candidates[idx];
        if (pos == 0) continue; // já adicionada
        bool ok = true;
        for (int epos : eventPositions) {
            int dist = abs(epos - pos);
            // considerar distancia circular tambem
            dist = std::min(dist, slots - dist);
            if (dist < minDist) { ok = false; break; }
        }
        if (ok) eventPositions.push_back(pos);
    }
    // fallback: se nao conseguimos posições espaçadas, preencha com os primeiros
    if ((int)eventPositions.size() < needed) {
        for (int i = 0; i < needed && i < (int)candidates.size(); ++i) {
            if (std::find(eventPositions.begin(), eventPositions.end(), candidates[i]) == eventPositions.end())
                eventPositions.push_back(candidates[i]);
        }
    }
    
    // MANTER os mesmos eventos
    events = {
        Event("Dividendos! Receba $100", 100.0f, 0),
        Event("Imposto de Renda! Pague $50", -50.0f, 0),
        Event("Fundo desvalorizou! Pague $30", -30.0f, 0),
        Event("Acao valorizou! Receba $80", 80.0f, 0),
    Event("Bolsa em alta! Avance 2 casas", 0.0f, 2),
    Event("Investidores estrangeiros estão investindo no país! Bolsa sobe 40%", 0.0f, 0, 40.0f),
    // Novos eventos de noticias: grande queda e boa noticia
    Event("Notícia muito ruim! Mercado -50%", 0.0f, 0, -50.0f),
    Event("Boa notícia! Mercado +10%", 0.0f, 0, 10.0f),
    // Eventos setoriais adicionais (commodities/retail/banks)
    Event("Crise do minério: Commodities -30%", 0.0f, 0, -30.0f, "commodities"),
    Event("Alta do varejo: Varejo +12%", 0.0f, 0, 12.0f, "retail"),
    Event("Problema nos bancos: Bancos -20%", 0.0f, 0, -20.0f, "banks"),
    Event("Campanha promocional no varejo: Varejo +8%", 0.0f, 0, 8.0f, "retail"),
    Event("Relatorio positivo de banco: Bancos +6%", 0.0f, 0, 6.0f, "banks"),
    Event("Quebra de mineradora: Commodities -18%", 0.0f, 0, -18.0f, "commodities"),
        Event("FII pagou dividendos! Receba $60", 60.0f, 0),
        Event("Taxa de administracao! Pague $40", -40.0f, 0)
    };

    // Constrói um mapeamento determinístico posição -> índice em properties para que buscas sejam robustas.
    // Inicializa o mapeamento com -1 (sem propriedade)
    positionToProperty.assign(slots, -1);
    // Garante que propertyPositions tenha uma entrada para cada propriedade (o fallback acima deve ter preenchido)
    for (size_t i = 0; i < propertyPositions.size() && i < properties.size(); ++i) {
        int pos = propertyPositions[i];
        if (pos >= 0 && pos < slots) positionToProperty[pos] = (int)i;
    }

    // Garante que toda posição do tabuleiro seja atribuída a uma propriedade ou a um evento.
    // Isso evita mostrar casas vazias sem propriedade nem evento.
    for (int pos = 0; pos < slots; ++pos) {
        bool hasProperty = (positionToProperty[pos] != -1);
        bool hasEvent = (std::find(eventPositions.begin(), eventPositions.end(), pos) != eventPositions.end());
        if (!hasProperty && !hasEvent) {
            eventPositions.push_back(pos);
        }
    }

}


std::shared_ptr<Property> Board::getPropertyAtPosition(int position) const {
    if (position < 0 || position >= totalPositions) return nullptr;
    if (positionToProperty.empty()) return nullptr;
    int idx = positionToProperty[position];
    if (idx >= 0 && idx < (int)properties.size()) return properties[idx];
    return nullptr;
}

bool Board::isEventPosition(int position) const {
    for (int pos : eventPositions) {
        if (pos == position) return true;
    }
    return false;
}

Event Board::getRandomEvent() const {
    if (events.empty()) {
        return Event("Nenhum evento", 0.0f, 0);
    }
    
    int randomIndex = GetRandomValue(0, events.size() - 1);
    return events[randomIndex];
}

void Board::updateMarketPrices() {
    // Flutuações de mercado aleatórias desativadas para simplificar a jogabilidade.
    // Os preços serão atualizados apenas via mudanças dirigidas pela SELIC
    // (Board::applyMarketShift) e por eventos explícitos que chamam applyMarketShift.
    // Esta função intencionalmente não faz nada agora.
    (void)properties; // evita aviso de variável não usada
}

static bool isHighDYAsset(const std::shared_ptr<Property>& property) {
    // FIIs are considered high DY
    // FIIs são considerados de alto DY
    if (std::dynamic_pointer_cast<FII>(property)) return true;

    // For stocks, consider banks and commodity-related tickers as high-DY
    // Para ações, considere bancos e tickers relacionados a commodities como alto DY
    if (auto s = std::dynamic_pointer_cast<Stock>(property)) {
        std::string ticker = s->getTicker();
        // Normalize to uppercase (tickers are already uppercase, but safe)
        for (auto &c : ticker) c = toupper((unsigned char)c);

        const std::vector<std::string> highDySubstr = {
            "BB", "BBAS", "BBDC", "ITUB", "ITAU", // bancos
            "PETR", "VALE", "CMIN", "GGBR", "CSN"  // commodities/mineracao/metal/energia
        };
        for (const auto &sub : highDySubstr) {
            // Check if the ticker contains any of the high DY substrings
            // Verifica se o ticker contém algum dos substrings de alto DY
            if (ticker.find(sub) != std::string::npos) return true;
        }
    }

    return false;
}

void Board::applyMarketShift(float percentHighDY, float percentOthers) {
    // Aplica mudanças de preço diferenciadas para ativos de alto DY e outros
    for (auto& property : properties) {
        if (isHighDYAsset(property)) {
            property->updatePrice(percentHighDY);
            property->updateRent(percentHighDY * 0.7f);
        } else {
            property->updatePrice(percentOthers);
            property->updateRent(percentOthers * 0.7f);
        }
    }
}

// Helpers para detectar pertencimento a setores
static bool isCommodityAsset(const std::shared_ptr<Property>& property) {
    if (auto s = std::dynamic_pointer_cast<Stock>(property)) {
        std::string t = s->getTicker();
        for (auto &c : t) c = toupper((unsigned char)c);
        const std::vector<std::string> commodities = {"VALE", "CMIN", "GGBR", "CSN"};
        for (auto &sub : commodities) if (t.find(sub) != std::string::npos) return true;
    }
    return false;
}

static bool isRetailAsset(const std::shared_ptr<Property>& property) {
    if (auto s = std::dynamic_pointer_cast<Stock>(property)) {
        std::string t = s->getTicker();
        for (auto &c : t) c = toupper((unsigned char)c);
        const std::vector<std::string> retail = {"MGLU", "LREN", "LAME", "AMER"};
        for (auto &sub : retail) if (t.find(sub) != std::string::npos) return true;
    }
    return false;
}

static bool isBankAsset(const std::shared_ptr<Property>& property) {
    // FIIs are not banks
    // FIIs não são bancos
    if (auto s = std::dynamic_pointer_cast<Stock>(property)) {
        std::string t = s->getTicker();
        for (auto &c : t) c = toupper((unsigned char)c);
        const std::vector<std::string> banks = {"BB", "BBAS", "ITUB", "BBDC", "ITAU"};
        for (auto &sub : banks) if (t.find(sub) != std::string::npos) return true;
    }
    return false;
}

void Board::applyMarketShiftForSector(const std::string& sector, float percent) {
    // Aplica um shift percentual a um setor específico (ex: "commodities").
    std::string s = sector;
    for (auto &c : s) c = tolower((unsigned char)c);
    for (auto& property : properties) {
        bool match = false;
        if (s == "commodities") match = isCommodityAsset(property);
        else if (s == "retail" || s == "varejo") match = isRetailAsset(property);
        else if (s == "banks" || s == "bank") match = isBankAsset(property);
        else if (s == "all" || s.empty()) match = true;

        if (match) {
            property->updatePrice(percent);
            property->updateRent(percent * 0.7f);
        }
    }
}

void Board::resetMarketPrices() {
    // Restaura preços ao valor base.
    for (auto& property : properties) {
        property->resetToBasePrice();
    }
}