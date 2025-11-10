#include "Board.hpp"
#include "Stock.hpp"
#include "FII.hpp"
#include <random>
#include <algorithm>
#include <cctype>

Board::Board() : totalPositions(40) {  
    initializeProperties();
}

// Função utilitária para verificar se um número é primo
static bool isPrime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

void Board::initializeProperties() {
    properties.clear();
    
    // Posições não-primas (0-39): onde ficam as propriedades
    // Primos: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 = eventos
    // Não-primos: 0, 1, 4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20, 21, 22, 24, 25, 26, 27, 28, 30, 32, 33, 34, 35, 36, 38, 39
    std::vector<int> nonPrimePositions = {0, 1, 4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20, 21, 22, 24, 25, 26, 27, 28, 30, 32, 33, 34, 35, 36, 38, 39};
    
    int posIndex = 0; // Índice para percorrer as posições não-primas
    
    // Criação das propriedades com posição específica
    properties.push_back(std::make_shared<Stock>("BBAS3", "Banco do Brasil", 60.0f, 7.0f, (Color){52, 152, 219, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("PETR4", "Petrobras", 60.0f, 6.0f, (Color){39, 174, 96, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("VALE3", "Vale S.A.", 100.0f, 6.0f, (Color){142, 68, 173, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<FII>("HGLG11", "CSHG Logística", 140.0f, 10.0f, (Color){230, 126, 34, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<FII>("XPML11", "XP Malls", 160.0f, 12.0f, (Color){22, 160, 133, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<FII>("HGRU11", "CSHG Renda Urbana", 180.0f, 14.0f, (Color){155, 89, 182, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("ITUB4", "Itaú Unibanco", 120.0f, 7.0f, (Color){46, 204, 113, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("ABEV3", "Ambev", 80.0f, 3.0f, (Color){241, 196, 15, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("BBDC4", "Bradesco", 110.0f, 6.5f, (Color){231, 76, 60, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("PETR3", "Petrobras PN", 70.0f, 6.0f, (Color){26, 188, 156, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("MGLU3", "Magazine Luiza", 95.0f, 3.5f, (Color){52, 73, 94, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("GGBR4", "Gerdau PN", 85.0f, 5.5f, (Color){117, 80, 123, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("NATU3", "Natura", 90.0f, 3.8f, (Color){39, 174, 96, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("CSAN3", "Cosan", 75.0f, 5.5f, (Color){41, 128, 185, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("CMIN3", "CSN Mineração", 65.0f, 6.0f, (Color){142, 68, 173, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("LREN3", "Lojas Renner", 105.0f, 4.0f, (Color){52, 152, 219, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("RENT3", "Localiza/Movida", 88.0f, 3.6f, (Color){142, 68, 173, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("BRFS3", "BRF", 72.0f, 3.2f, (Color){241, 196, 15, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("AZUL4", "Azul Linhas Aereas", 68.0f, 2.9f, (Color){231, 76, 60, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<Stock>("WEGE3", "Weg", 140.0f, 5.0f, (Color){26, 188, 156, 255}, nonPrimePositions[posIndex++]));
    
    // FIIs adicionais
    properties.push_back(std::make_shared<FII>("KNRI11", "Kinea Renda Imobiliaria", 130.0f, 10.0f, (Color){149, 165, 166, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<FII>("VISC11", "Vinci Shopping Centers", 150.0f, 11.0f, (Color){211, 84, 0, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<FII>("MXRF11", "Maxi Renda", 110.0f, 9.0f, (Color){44, 62, 80, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<FII>("HABT11", "Habitasul FII", 95.0f, 9.0f, (Color){22, 160, 133, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<FII>("RBRP11", "RBR Properties", 120.0f, 10.0f, (Color){39, 174, 96, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<FII>("BTLG11", "BTG Pactual Logística", 105.0f, 9.5f, (Color){192, 57, 43, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<FII>("HCRI11", "HCR Imóveis", 115.0f, 10.5f, (Color){155, 89, 182, 255}, nonPrimePositions[posIndex++]));
    properties.push_back(std::make_shared<FII>("ALZR11", "Alianza Trust Renda", 100.0f, 9.2f, (Color){52, 73, 94, 255}, nonPrimePositions[posIndex++]));
    
    // Eventos fixos
    events = {
        Event("Dividendos! Receba $100", 100.0f, 0),
        Event("Imposto de Renda! Pague $50", -50.0f, 0),
        Event("Fundo desvalorizou! Pague $30", -30.0f, 0),
        Event("Acao valorizou! Receba $80", 80.0f, 0),
        Event("Bolsa em alta! Avance 2 casas", 0.0f, 2),
        Event("Investidores estrangeiros estão investindo no país! Bolsa sobe 40%", 0.0f, 0, 40.0f),
        Event("Notícia muito ruim! Mercado -50%", 0.0f, 0, -50.0f),
        Event("Boa notícia! Mercado +10%", 0.0f, 0, 10.0f),
        Event("Crise do minério: Commodities -30%", 0.0f, 0, -30.0f, "commodities"),
        Event("Alta do varejo: Varejo +12%", 0.0f, 0, 12.0f, "retail"),
        Event("Problema nos bancos: Bancos -20%", 0.0f, 0, -20.0f, "banks"),
        Event("Campanha promocional no varejo: Varejo +8%", 0.0f, 0, 8.0f, "retail"),
        Event("Relatorio positivo de banco: Bancos +6%", 0.0f, 0, 6.0f, "banks"),
        Event("Quebra de mineradora: Commodities -18%", 0.0f, 0, -18.0f, "commodities"),
        Event("FII pagou dividendos! Receba $60", 60.0f, 0),
        Event("Taxa de administracao! Pague $40", -40.0f, 0)
    };
}

// MÉTODOS SIMPLIFICADOS
std::shared_ptr<Property> Board::getPropertyAtPosition(int position) const {
    // Agora é muito simples: se a posição é número primo, não tem propriedade
    if (isPrime(position)) return nullptr;
    
    // Busca linear pela propriedade na posição (poderia ser otimizado com hash map se necessário)
    for (const auto& property : properties) {
        if (property->getPosition() == position) {
            return property;
        }
    }
    return nullptr;
}

bool Board::isEventPosition(int position) const {
    // Números primos são posições de eventos
    return isPrime(position);
}

Event Board::getRandomEvent() const {
    if (events.empty()) {
        return Event("Nenhum evento", 0.0f, 0);
    }
    
    int randomIndex = GetRandomValue(0, events.size() - 1);
    return events[randomIndex];
}

// FUNÇÕES AUXILIARES PARA CATEGORIZAÇÃO DE ATIVOS
static bool isHighDYAsset(const std::shared_ptr<Property>& property) {
    // FIIs são considerados de alto DY
    if (std::dynamic_pointer_cast<FII>(property)) return true;

    // Para ações, considere bancos e tickers relacionados a commodities como alto DY
    if (auto s = std::dynamic_pointer_cast<Stock>(property)) {
        std::string ticker = s->getTicker();
        // Normalize to uppercase (tickers already uppercase, but safe)
        for (auto &c : ticker) c = toupper((unsigned char)c);

        const std::vector<std::string> highDySubstr = {
            "BB", "BBAS", "BBDC", "ITUB", "ITAU", // bancos
            "PETR", "VALE", "CMIN", "GGBR", "CSN"  // commodities/mineracao/metal/energia
        };
        for (const auto &sub : highDySubstr) {
            if (ticker.find(sub) != std::string::npos) return true;
        }
    }
    return false;
}

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
    if (auto s = std::dynamic_pointer_cast<Stock>(property)) {
        std::string t = s->getTicker();
        for (auto &c : t) c = toupper((unsigned char)c);
        const std::vector<std::string> banks = {"BB", "BBAS", "ITUB", "BBDC", "ITAU"};
        for (auto &sub : banks) if (t.find(sub) != std::string::npos) return true;
    }
    return false;
}

// MÉTODOS DE MANIPULAÇÃO DE MERCADO
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

void Board::applyMarketShiftForSector(const std::string& sector, float percent) {
    // Aplica um shift percentual a um setor específico
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
    // Restaura preços ao valor base
    for (auto& property : properties) {
        property->resetToBasePrice();
    }
}