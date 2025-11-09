#include "Player.hpp"

Player::Player(const std::string& playerName, float startingMoney) 
    : name(playerName), money(startingMoney), position(0), lapCount(0) {}

void Player::move(int steps) {
    int oldPosition = position;
    position += steps;
    
    // Simple board wrap-around (assuming 40 positions like Monopoly)
    if (position >= 40) {
        position = position % 40;
        if (oldPosition < 40) {
            incrementLapCount();
        }
    }
    
    if (position < 0) {
        position = 40 + (position % 40);
    }
}
