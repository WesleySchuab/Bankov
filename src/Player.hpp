#pragma once
#include <string>
#include <vector>
#include <map>

class Player {
private:
    std::string name;
    float money;
    int position;
    int lapCount;

public:
    Player(const std::string& playerName, float startingMoney = 1000.0f);
    
    // Getters
    std::string getName() const { return name; }
    float getMoney() const { return money; }
    int getPosition() const { return position; }
    int getLapCount() const { return lapCount; }
    
    // Money operations
    void addMoney(float amount) { money += amount; }
    void removeMoney(float amount) { money -= amount; }
    bool canAfford(float amount) const { return money >= amount; }
    
    // Movement
    void move(int steps);
    void setPosition(int newPosition) { position = newPosition; }
    
    // Lap tracking
    void incrementLapCount() { lapCount++; }
};
