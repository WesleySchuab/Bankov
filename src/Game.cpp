#include "Game.hpp"
#include "raylib.h"

Game::Game() {
    // inicializaçao da janela e recursos
    InitWindow(800, 600, "AcoesImobiliarias - Esqueleto");
    SetTargetFPS(60);
}

Game::~Game() {
    // limpeza de recursos
    CloseWindow();
}

void Game::run() {
    // loop principal do jogo
    while (!WindowShouldClose()) {
        handleInput();
        update();
        render();
    }
}

void Game::handleInput() {
    // aqui vamos tratar teclas (ESPACO, C, N, I, V, etc)
}

void Game::update() {
    // lógica por frame
}

void Game::render() {
    // desenha a tela
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Esqueleto rodando - implemente a UI", 20, 20, 20, BLACK);
    EndDrawing();
}
