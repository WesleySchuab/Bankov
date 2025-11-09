#pragma once
#include <memory>

// Definição dos estados do jogo
enum class GameState { PLAYER_TURN, BUY_PROPERTY, INVEST_PROPERTY, GAME_OVER };

class Game {
    // métodos públicos
public:
    Game(); // construtor
    ~Game(); // destrutor
    void run(); // loop principal do jogo

private:
    void handleInput();// trata entrada do usuário
    void update(); // lógica do jogo
    void render(); // desenha a tela
};