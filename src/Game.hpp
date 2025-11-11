#pragma once
#include "raylib.h"
#include "Player.hpp"
#include "Board.hpp"
#include "Property.hpp"
#include <vector>
#include <memory>
#include <string>
#include <map>

class Game {
private:
    std::vector<std::shared_ptr<Player>> players;
    Board board;
    
    int currentPlayerIndex;
    int lastDiceRoll;
    std::string message;
    bool isEventMessage; // Flag para identificar se é mensagem de evento
    bool waitingForPurchase;
    std::shared_ptr<Property> currentProperty;
    int roundsCompleted;
    /** Contador global de voltas completas por qualquer jogador (usado para indicar proximidade do fim). */
    int globalLapsCompleted;
    
    bool gameRunning;
    enum class GameState {
        PLAYER_TURN,
        BUY_PROPERTY,
        INVEST_PROPERTY,
        WAITING_MESSAGE,
        GAME_OVER
    };
    GameState currentState;

    // NOVO: Fontes personalizadas
    Font fonteTitulo;
    Font fonteTexto;
    Font fontePequena;

public:
    Game();
    ~Game();
    
    /** Inicia o loop principal do jogo. */
    void run();
    /** Reinicia o jogo para o estado inicial. */
    void restart();
    /** Atualiza o estado do jogo (lógica por frame). */
    void update();
    /** Renderiza a cena (UI + tabuleiro). */
    void render();
    
    /** Inicializa jogadores padrão (usado no construtor). */
    void initializePlayers();
    /** Avança para o próximo jogador/turno. */
    void nextTurn();
    /** Verifica se uma volta completa foi concluída e contabiliza rounds. */
    void checkRoundCompletion();
    /** Processa flutuações de mercado (SELIC, eventos periódicos). */
    void processMarketFluctuations();
    
    /** Lê entradas do usuário (teclado) e dispara ações. */
    void handleInput();
    /** Rola o dado e aplica movimento/efeitos da casa. */
    void processDiceRoll();
    /** Tenta comprar a propriedade atual para o jogador atual. */
    void processPropertyPurchase();
    /** Investe na propriedade atual (aumenta preço/aluguel). */
    void processInvestProperty();
    /** Vende a propriedade atual (se for do jogador). */
    void processSellProperty();
    /** O jogador atual passa a vez (não compra). */
    void processPassTurn();

    // extra holdings per player: ticker/name -> count (additional units bought)
    std::vector<std::map<std::string,int>> extraHoldings;

    // cost basis tracking per player: ticker/name -> {units, totalCostPaid}
    struct CostBasis {
        int units = 0;
        float totalCost = 0.0f; // sum of money paid for these units
    };
    std::vector<std::map<std::string, CostBasis>> costBasis;
    // SELIC / macro variables (displayed above board)
    float selicRate;
    float selicDecreaseStep; // how much SELIC decreases every interval (percentage points)
    // quando SELIC diminui damos dois tipos de subida: uma para ativos de alto DY
    // (FIIs e ações de bancos/commodities) e outra para os demais ativos
    float selicAssetsBumpPercentHighDY; // percent for FIIs / bank & commodity stocks
    float selicAssetsBumpPercentOthers; // percentual para os demais ativos
    int selicDecreaseInterval; // number of rounds between SELIC decreases

    // calculate total assets (money + owned tiles + extra holdings valued at current prices)
    float calculatePlayerTotalAssets(int playerIndex) const;
    // calculate only asset value (owned properties + extra holdings), excluding cash
    float calculatePlayerAssetsValue(int playerIndex) const;

    // history of patrimony (assets value) per player tracked per round
    std::vector<std::vector<float>> patrimonyHistory;
    
    //void renderUI();
    void renderPlayers();
    void renderBoard();
    void renderInstructions();
    // Mensagens destacadas exibidas entre o tabuleiro e as instruções
    void renderMessageBox();
    /** Define a mensagem destacada (persistente até ser sobrescrita). */
    void setMessage(const std::string &msg, bool isEvent = false);
    /** Anexa texto à mensagem atual (não expira automaticamente). */
    void appendMessage(const std::string &more);
    /** Renderiza um mini-gráfico do histórico de patrimônio por jogador. */
    void renderPatrimonyChart();

private:
    // (mensagens agora persistem até serem sobrescritas)
    /** Amostra o patrimônio atual de todos os jogadores e armazena em `patrimonyHistory`. */
    void samplePatrimonyHistory();
};