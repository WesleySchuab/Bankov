#pragma once
#include <memory>

enum class GameState { PLAYER_TURN, BUY_PROPERTY, INVEST_PROPERTY, GAME_OVER };

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void handleInput();
    void update();
    void render();

};