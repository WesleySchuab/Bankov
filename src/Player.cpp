#include "Player.hpp"
#include "Property.hpp"
#include <algorithm>

Player::Player(int id, const std::string& name, Color color, float initialMoney)
    : id(id), name(name), color(color), money(initialMoney), position(0), lapsCompleted(0) {}

void Player::move(int steps) {
    // Faz o wrap-around para respeitar o tamanho do tabuleiro (40 posições)
    int oldPos = position;
    position = (position + steps) % 40;
    /*Se o jogador estava em 38 e rolou 5: (38 + 5) % 40 == 3. 
    38+5 = 43, 43 % 40 = 3.
    Game detecta que a nova posição (3) < posição anterior (38) e conclui que o jogador completou uma volta.*/
    // Se deu wrap (posição atual menor que a anterior), não incrementamos aqui —
    // a lógica de contagem de voltas é controlada pelo Game para permitir bonificação
}

void Player::incrementLaps() {
    // Incrementa o contador de voltas completas do jogador
    lapsCompleted++;
}

int Player::getLapsCompleted() const {
    // Retorna quantas voltas completas o jogador já realizou
    return lapsCompleted;
}

void Player::addMoney(float amount) {
    // Adiciona (ou subtrai) dinheiro do jogador
    money += amount;
}

bool Player::canAfford(float amount) const {
    // Verifica se o jogador pode pagar um valor
    return money >= amount;
}

void Player::buyProperty(std::shared_ptr<Property> property) {
    // Executa a compra de uma propriedade pelo jogador
    if (property && property->canBeBought(money)) { // verifica se pode comprar
        money -= property->getPrice(); // debita o preço
        property->setOwner(id); // define o dono
        properties.push_back(property); // adiciona à lista de propriedades
    }
}

void Player::sellProperty(std::shared_ptr<Property> property) {
    // Vende uma propriedade do jogador (se possuir)
    if (!property) return; // verifica validade
    // procura na lista de propriedades
    auto it = std::find_if(properties.begin(), properties.end(), [&](const std::shared_ptr<Property>& p) {
        return p.get() == property.get();
    });
    if (it != properties.end()) {
        // vender volta o preço atual ao jogador
        float received = property->getPrice(); // valor recebido na venda
        money += received;
        property->setOwner(0); // propriedade volta a não ter dono
        properties.erase(it); // remove da lista de propriedades
    }
}

void Player::payRent(std::shared_ptr<Property> property) {
    // Paga aluguel/dividendo relativo a uma propriedade de outro jogador
    if (property && property->getOwner() != id && property->getOwner() != 0) { // se não for do próprio jogador e tiver dono
        float rent = property->getRent();
        if (money >= rent) {
            money -= rent;
        }
    }
}

float Player::calculateTotalAssets() const {
    // Calcula ativos totais (dinheiro + valor das propriedades)
    float total = money;
    for (const auto& prop : properties) {
        total += prop->getPrice();
    }
    return total;
}

void Player::reset() {
    money = 1500.0f;
    position = 0;
    properties.clear();
    lapsCompleted = 0;
}

void Player::setPosition(int pos) {
    position = pos;
}