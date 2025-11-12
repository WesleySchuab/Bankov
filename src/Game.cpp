#include "Game.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "Stock.hpp"
#include "FII.hpp"
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>

Game::Game() 
    : currentPlayerIndex(0), lastDiceRoll(0), 
      message(""), isEventMessage(false),
      waitingForPurchase(false), roundsCompleted(0), 
    globalLapsCompleted(0),
      gameRunning(true), currentState(GameState::PLAYER_TURN) {
    
    InitWindow(1300, 1000, "Acoes & Fundos Imobiliarios");
    SetTargetFPS(60);
    
    // CARREGAR FONTES PERSONALIZADAS
    fonteTitulo = LoadFont("assets/fonts/Lato-Black.ttf");
    fonteTexto = LoadFont("assets/fonts/Lato-Black.ttf"); 
    fontePequena = LoadFont("assets/fonts/Lato-Black.ttf");
    fonteNegrito = LoadFont("assets/Lato/Lato-Bold.ttf");
    
    // Verificar se as fontes carregaram corretamente
    if (fonteTitulo.texture.id == 0) {
        printf("ERRO: Nao foi possivel carregar a fonte Lato-Black.ttf\n");
        printf("Verifique se o arquivo esta em: assets/fonts/Lato-Black.ttf\n");
        fonteTitulo = GetFontDefault();
        fonteTexto = GetFontDefault();
        fontePequena = GetFontDefault();
    } else {
        printf("SUCESSO: Fonte Lato-Black carregada!\n");
    }
    
    // Verificar fonte negrito separadamente
    if (fonteNegrito.texture.id == 0) {
        printf("ERRO: Nao foi possivel carregar a fonte Lato-Bold.ttf\n");
        printf("Verifique se o arquivo esta em: assets/Lato/Lato-Bold.ttf\n");
        fonteNegrito = GetFontDefault();
    } else {
        printf("SUCESSO: Fonte Lato-Bold carregada!\n");
    }
    
    initializePlayers();
    // inicializar extraHoldings para cada jogador
    extraHoldings.resize(players.size());
    // inicializar costBasis para cada jogador
    costBasis.resize(players.size());
    // inicializar historico de patrimonio por jogador
    patrimonyHistory.resize(players.size());

    // SELIC defaults
    selicRate = 13.75f; // starting SELIC (percent)
    selicDecreaseStep = 0.25f; // decreases by 0.25 percentage points every interval
    // when SELIC decreases, apply two bumps: high-DY assets +5%, others +10%
    selicAssetsBumpPercentHighDY = 5.0f;
    selicAssetsBumpPercentOthers = 10.0f;
    selicDecreaseInterval = 5; // diminui SELIC a cada 5 rodadas (configurado pelo usuario)
}

// Mensagens de status/compra em destaque entre o tabuleiro e as instruções
void Game::renderMessageBox() {
    // Recalcula as mesmas métricas de layout usadas por renderBoard para posicionar a caixa logo abaixo do tabuleiro
    int tabuleiroY = 540;
    int tabuleiroHeight = 220;
    int boardBottom = tabuleiroY + tabuleiroHeight + 60; // matches renderBoard usage

    int messageY = boardBottom + 20; // small gap below board
    int boxX = 40; // align to left margin
    int boxW = GetScreenWidth() - 80; // usa a largura disponível entre as margens
    int boxH = 80; // Altura reduzida para não cobrir o gráfico

    bool showYellow = false;
    std::string yellowText = "";
    if (waitingForPurchase && currentProperty) {
        showYellow = true;
        if (!message.empty()) {
            yellowText += " - ";
            yellowText += message;
        }
    } else if (!message.empty()) {
        showYellow = true;
        yellowText = message;
    }

    if (showYellow) {
        // Escolher cor de fundo baseada no tipo de mensagem
        Color backgroundColor = isEventMessage ? (Color){255, 218, 203, 255} : (Color){193, 225, 193, 255}; // Pêssego ou verde suave
        
        DrawRectangle(boxX, messageY, boxW, boxH, backgroundColor); // x, y, width, height, color (destaque da mensagem)
        
        // Quebra texto em múltiplas linhas se necessário
        int fontSize = 16;
        int lineHeight = fontSize + 4;
        int maxWidth = boxW - 20; // Margem normal
        int startX = boxX + 10; // Margem normal
        int startY = messageY + 10; // Margem normal
        
        // Escolher cor do texto baseada no fundo
        Color textColor = isEventMessage ? BLACK : BLACK; // Texto preto para ambos os fundos
        
        // Quebrar texto em palavras
        std::vector<std::string> words;
        std::string word = "";
        for (char c : yellowText) {
            if (c == ' ' || c == '\n') {
                if (!word.empty()) {
                    words.push_back(word);
                    word = "";
                }
                if (c == '\n') words.push_back("\n");
            } else {
                word += c;
            }
        }
        if (!word.empty()) words.push_back(word);
        
        // Renderizar palavras linha por linha
        std::string currentLine = "";
        int currentY = startY;
        for (const std::string& w : words) {
            if (w == "\n") {
                if (!currentLine.empty()) {
                    DrawTextEx(fonteTexto, currentLine.c_str(), (Vector2){(float)startX, (float)currentY}, fontSize, 1, textColor);
                    currentLine = "";
                    currentY += lineHeight;
                }
                currentY += lineHeight;
            } else {
                std::string testLine = currentLine.empty() ? w : currentLine + " " + w;
                Vector2 textSize = MeasureTextEx(fonteTexto, testLine.c_str(), fontSize, 1);
                
                if (textSize.x <= maxWidth) {
                    currentLine = testLine;
                } else {
                    // Linha atual está cheia, renderizar e começar nova linha
                    if (!currentLine.empty()) {
                        DrawTextEx(fonteTexto, currentLine.c_str(), (Vector2){(float)startX, (float)currentY}, fontSize, 1, textColor);
                        currentY += lineHeight;
                    }
                    currentLine = w;
                }
            }
        }
        // Renderizar última linha
        if (!currentLine.empty()) {
            DrawTextEx(fonteTexto, currentLine.c_str(), (Vector2){(float)startX, (float)currentY}, fontSize, 1, textColor);
            currentY += lineHeight;
        }
        
        // Mostrar instrução para continuar se estiver no estado WAITING_MESSAGE
        if (currentState == GameState::WAITING_MESSAGE) {
            currentY += 10;
            DrawTextEx(fontePequena, "Pressione [N] para continuar...", (Vector2){(float)startX, (float)currentY}, 14, 1, DARKBLUE);
        }
    }
}

Game::~Game() {
    // DESCARREGAR FONTES
    UnloadFont(fonteTitulo);
    UnloadFont(fonteTexto);
    UnloadFont(fontePequena);
    UnloadFont(fonteNegrito);
    
    CloseWindow();
}

void Game::run() {
    while (!WindowShouldClose()) {
        handleInput();
        update();
        render();
    }
}

void Game::handleInput() {
    // Quando em estado GAME_OVER, somente aceitar reinício (R)
    if (currentState == GameState::GAME_OVER) {
        if (IsKeyPressed(KEY_R)) {
            restart();
        }
        return;
    }
    
    // Quando esperando que o jogador leia a mensagem após evento
    if (currentState == GameState::WAITING_MESSAGE) {
        if (IsKeyPressed(KEY_N)) {
            currentState = GameState::PLAYER_TURN;
            nextTurn();
        }
        return;
    }
    
    if (IsKeyPressed(KEY_SPACE) && currentState == GameState::PLAYER_TURN) {
        processDiceRoll();
    }
    
    if (IsKeyPressed(KEY_C) && currentState == GameState::BUY_PROPERTY) {
        processPropertyPurchase();
    }
    
    if (IsKeyPressed(KEY_N) && currentState == GameState::BUY_PROPERTY) {
        processPassTurn();
    }
    
    // Investir em propriedade que já é sua (aumenta preço/aluguel)
    if (IsKeyPressed(KEY_I) && currentState == GameState::INVEST_PROPERTY) {
        processInvestProperty();
    }
    // Vender propriedade que é sua
    if (IsKeyPressed(KEY_V) && currentState == GameState::INVEST_PROPERTY) {
        processSellProperty();
    }
}

void Game::processDiceRoll() {
    lastDiceRoll = GetRandomValue(1, 6);
    auto currentPlayer = players[currentPlayerIndex];
    int posicaoAntiga = currentPlayer->getPosition();
    currentPlayer->move(lastDiceRoll);
    setMessage(currentPlayer->getName() + " rolou " + std::to_string(lastDiceRoll), false); // Marcar como mensagem normal (não-evento)

    if (currentPlayer->getPosition() < posicaoAntiga) {
        // Aplicar rendimento da Selic sobre o dinheiro não investido
        float dinheiroAtual = currentPlayer->getMoney();
        float rendimento = dinheiroAtual * (selicRate / 100.0f);
        currentPlayer->addMoney(rendimento);
        
        // Bonus da volta completa
        currentPlayer->addMoney(200);
        
        // Mensagem com detalhes do rendimento
        appendMessage(" - Ganhou $200 por completar uma volta!");
        if (rendimento > 0.1f) {
            appendMessage(" - Rendimento Selic (" + std::to_string(selicRate).substr(0, 4) + "%): +$" + std::to_string((int)rendimento));
        }
        
        // Reduzir a Selic após aplicar o rendimento
        selicRate = std::max(2.0f, selicRate - 0.5f);
        appendMessage(" - Selic reduzida para " + std::to_string(selicRate).substr(0, 4) + "%");
        
        // Incrementar o contador de voltas do jogador e verificar condição de fim
        currentPlayer->incrementLaps();
        // Incrementar contador global de voltas completas e de "rounds" (contagem usada para efeitos)
        globalLapsCompleted++;
        // Contador de "rounds" agora reflete voltas efetivamente completadas (apenas quando um jogador fecha a volta)
        roundsCompleted++;
        // Processar flutuações de mercado quando uma volta é completada
        processMarketFluctuations();
        std::string ordinal;
        if (globalLapsCompleted == 1) ordinal = "primeira";
        else if (globalLapsCompleted == 2) ordinal = "segunda";
        else if (globalLapsCompleted == 3) ordinal = "terceira";
        else if (globalLapsCompleted == 4) ordinal = "quarta";
        else ordinal = std::to_string(globalLapsCompleted);
        appendMessage(" - " + ordinal + " volta completa (" + std::to_string(globalLapsCompleted) + "/4)");
        int laps = currentPlayer->getLapsCompleted();
        // Se o jogador alcançou 4 voltas, apenas notificar — o jogo só termina quando
        // um jogador completar 5 voltas, momento em que comparamos patrimônios.
        if (laps == 4) {
            appendMessage(" - " + currentPlayer->getName() + " alcançou 4 voltas (próxima volta decidirá vencedor de patrimônio)");
        }
        // Se o jogador completou 5 (ou mais) voltas, decidir vencedor pelo maior patrimônio
        if (laps >= 5) {
            // comparar patrimônio total de todos os jogadores
            int winnerIdx = 0;
            float best = calculatePlayerTotalAssets(0);
            bool tie = false;
            for (int i = 1; i < (int)players.size(); ++i) {
                float val = calculatePlayerTotalAssets(i);
                if (val > best) { best = val; winnerIdx = i; tie = false; }
                else if (val == best) { tie = true; }
            }
            if (!tie) {
                setMessage(players[winnerIdx]->getName() + " venceu por maior patrimônio: $" + std::to_string((int)best));
            } else {
                // Empate: listar jogadores empatados
                std::string tieNames = "Empate entre: ";
                for (int i = 0; i < (int)players.size(); ++i) {
                    if (abs(calculatePlayerTotalAssets(i) - best) < 0.5f) {
                        if (tieNames.size() > 13) tieNames += ", ";
                        tieNames += players[i]->getName();
                    }
                }
                setMessage(tieNames + " com $" + std::to_string((int)best));
            }
            // Definir estado de GAME_OVER sem encerrar o loop de render para que possamos
            // mostrar o painel de vitória e permitir reinício.
            currentState = GameState::GAME_OVER;
            // Não alteramos gameRunning; rendereamos tela de GAME_OVER até reiniciar.
            return;
        }
    }

    int position = currentPlayer->getPosition();
    auto property = board.getPropertyAtPosition(position);

    if (property) {
        currentProperty = property;
        if (property->getOwner() == 0) {
            appendMessage(" - Pode comprar " + property->getName() + " por $" + std::to_string((int)property->getPrice()));
            currentState = GameState::BUY_PROPERTY;
            waitingForPurchase = true;
        } else if (property->getOwner() != currentPlayer->getId()) {
            // Tipo-específico de taxa quando parar em propriedade de outro jogador
            auto ownerPlayer = players[property->getOwner() - 1];
            float fee = 0.0f;
            std::string feeType = "taxa";

            // FII -> aluguel/dividendos
            if (auto f = std::dynamic_pointer_cast<FII>(property)) {
                fee = f->getRent();
                feeType = "aluguel";
                // Pre-message antes do pagamento
                appendMessage(" - Vai pagar " + feeType + " de $" + std::to_string((int)fee) + " para " + ownerPlayer->getName() + "...");
                // Owner recebe na forma de dividendos/JCP
                ownerPlayer->addMoney(fee);
                currentPlayer->addMoney(-fee);
                appendMessage(" Pago. (dividendos/JCP creditados ao proprietario).");
            }
            // AÇÕES -> diferenciar por ticker (PETR -> combustivel, BBAS -> juros)
            else if (auto s = std::dynamic_pointer_cast<Stock>(property)) {
                fee = s->getRent();
                std::string ticker = s->getTicker();
                if (ticker.find("PETR") != std::string::npos) {
                    feeType = "combustivel";
                    appendMessage(" - Vai pagar " + feeType + " de $" + std::to_string((int)fee) + " para " + ownerPlayer->getName() + "...");
                    ownerPlayer->addMoney(fee);
                    currentPlayer->addMoney(-fee);
                    appendMessage(" Pago.");
                } else if (ticker.find("BBAS") != std::string::npos ||
                           ticker.find("BB") != std::string::npos) {
                    feeType = "juros";
                    appendMessage(" - Vai pagar " + feeType + " de $" + std::to_string((int)fee) + " para " + ownerPlayer->getName() + "...");
                    ownerPlayer->addMoney(fee);
                    currentPlayer->addMoney(-fee);
                    appendMessage(" Pago. (creditado como juros/JCP).");
                } else {
                    feeType = "compra de produto";
                    appendMessage(" - Vai pagar " + feeType + " de $" + std::to_string((int)fee) + " para " + ownerPlayer->getName() + "...");
                    ownerPlayer->addMoney(fee);
                    currentPlayer->addMoney(-fee);
                    appendMessage(" Pago.");
                }
            }
            // Caso genérico (se outra implementação for adicionada no futuro)
            else {
                fee = property->getRent();
                appendMessage(" - Vai pagar $" + std::to_string((int)fee) + " para " + ownerPlayer->getName() + "...");
                ownerPlayer->addMoney(fee);
                currentPlayer->addMoney(-fee);
                appendMessage(" Pago.");
            }

            // Nota: a condição de fim por patrimônio foi removida.
            // O jogo agora termina quando um jogador completar 4 voltas (controlado em processDiceRoll).
        }
        else {
            // Property is owned by current player: can invest or sell
            appendMessage(" - Voce ja e proprietario. Pressione I para investir (aumenta valor/aluguel) ou V para vender.");
            currentState = GameState::INVEST_PROPERTY;
            waitingForPurchase = false;
        }
    } else if (board.isEventPosition(position)) {
    // Usa a lista de eventos do tabuleiro; eventos agora nao movem jogadores, apenas
    // aplicam efeitos financeiros ou de mercado (setor/percentual)
        Event randomEvent = board.getRandomEvent();
        randomEvent.execute(*currentPlayer);
        setMessage(randomEvent.getDescription(), true); // Marcar como mensagem de evento
        float marketPct = randomEvent.getMarketPercent();
        std::string sector = randomEvent.getSectorTarget();
        if (marketPct != 0.0f) {
            if (!sector.empty()) {
                board.applyMarketShiftForSector(sector, marketPct);
                appendMessage(" Mercado ajustado em " + std::to_string((int)marketPct) + "% para setor " + sector + ".");
            } else {
                board.applyMarketShift(marketPct, marketPct);
                appendMessage(" Mercado ajustado em " + std::to_string((int)marketPct) + "% para todos os ativos.");
            }
        }
        // Entrar no estado de espera para que o jogador leia a mensagem
        currentState = GameState::WAITING_MESSAGE;
        // Atualiza o estado do jogador após o evento
    }
        // Amostrar patrimonio dos jogadores a cada jogada (melhora formação do gráfico)
        samplePatrimonyHistory();

    // Avance automaticamente para o próximo turno apenas quando não estivermos
    // aguardando uma compra e não estivermos no estado INVEST_PROPERTY
    // (assim o jogador pode pressionar I ou V para agir sobre sua propriedade).
    if (!waitingForPurchase && currentState != GameState::INVEST_PROPERTY) {
        nextTurn();
    }
}

void Game::processPropertyPurchase() {
    auto currentPlayer = players[currentPlayerIndex];
    
    if (currentProperty && currentProperty->canBeBought(currentPlayer->getMoney())) {
        // Record transaction price for cost-basis
        float pricePaid = currentProperty->getPrice();
        std::string key;
        if (auto s = std::dynamic_pointer_cast<Stock>(currentProperty)) key = s->getTicker();
        else if (auto f = std::dynamic_pointer_cast<FII>(currentProperty)) key = f->getTicker();
        else key = currentProperty->getName();
        // Update cost basis
        auto &cb = costBasis[currentPlayerIndex][key];
        cb.units += 1;
        cb.totalCost += pricePaid;

        currentPlayer->buyProperty(currentProperty);
        setMessage("COMPROU " + currentPlayer->getName() + " comprou " + currentProperty->getName() + "!");
    } else {
        setMessage("ERRO: Dinheiro insuficiente para " + currentProperty->getName());
    }
    
    waitingForPurchase = false;
    currentProperty.reset();
    nextTurn();
}

void Game::processPassTurn() {
    setMessage("PASSOU " + players[currentPlayerIndex]->getName() + " passou a vez");
    waitingForPurchase = false;
    currentProperty.reset();
    nextTurn();
}

void Game::nextTurn() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    currentState = GameState::PLAYER_TURN;
    checkRoundCompletion();
}

void Game::checkRoundCompletion() {
    if (currentPlayerIndex == 0) {
        // roundsCompleted is incremented when a player completes a lap (processDiceRoll).
        // Aqui apenas verificamos se alcançamos o limite para finalizar o jogo.
        // Se completamos 5 rodadas (rounds) encerrar o jogo comparando patrimônio
        if (roundsCompleted >= 5 && currentState != GameState::GAME_OVER) {
            // decidir vencedor pelo maior patrimônio
            int winnerIdx = 0;
            float best = calculatePlayerTotalAssets(0);
            bool tie = false;
            for (int i = 1; i < (int)players.size(); ++i) {
                float val = calculatePlayerTotalAssets(i);
                if (val > best) { best = val; winnerIdx = i; tie = false; }
                else if (val == best) { tie = true; }
            }
            if (!tie) {
                setMessage(players[winnerIdx]->getName() + " venceu por maior patrimônio após 5 rodadas: $" + std::to_string((int)best));
            } else {
                std::string tieNames = "Empate entre: ";
                for (int i = 0; i < (int)players.size(); ++i) {
                    if (abs(calculatePlayerTotalAssets(i) - best) < 0.5f) {
                        if (tieNames.size() > 13) tieNames += ", ";
                        tieNames += players[i]->getName();
                    }
                }
                setMessage(tieNames + " com $" + std::to_string((int)best));
            }
            // zera posicoes dos jogadores para mostrar início
            for (auto &p : players) p->setPosition(0);
            currentState = GameState::GAME_OVER;
        }
    }
}

void Game::processMarketFluctuations() {
    // dirigidas pela SELIC.

    // A cada selicDecreaseInterval rodadas, a SELIC diminui e os ativos recebem um aumento
    if (roundsCompleted > 0 && (roundsCompleted % selicDecreaseInterval) == 0) {
        float oldSelic = selicRate;
        selicRate = std::max(0.0f, selicRate - selicDecreaseStep);
        // aplica um aumento de mercado diferenciado para os ativos quando a SELIC diminui
        board.applyMarketShift(selicAssetsBumpPercentHighDY, selicAssetsBumpPercentOthers);

    // Informar o jogador especificamente sobre a mudança da SELIC
        // criar string em uma passada para evitar múltiplos appends
        setMessage(std::string("SELIC caiu de ") + std::to_string((int)oldSelic) + "% para " + std::to_string((int)selicRate) + "% - FIIs/bancos/commodities subiram " + std::to_string((int)selicAssetsBumpPercentHighDY) + "% e os demais ativos subiram " + std::to_string((int)selicAssetsBumpPercentOthers) + "%");
    }

    // Registrar o valor dos ativos (sem caixa) no historico de cada jogador
    for (int i = 0; i < (int)players.size(); ++i) {
        float assetsValue = calculatePlayerAssetsValue(i);
        patrimonyHistory[i].push_back(assetsValue);
    }
}

// Atualiza a lógica por frame — sem temporizadores de mensagem (mensagens persistem)
void Game::update() {
    // Mantemos o frame-time disponível para futuras expansões; não usamos temporizadores de mensagem
    (void)GetFrameTime();
}

void Game::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);   
    renderPlayers();
    renderBoard();
    renderMessageBox();
    renderPatrimonyChart();
    renderInstructions();
    // Se estivermos em GAME_OVER, desenhar overlay com botão de reinício
    if (currentState == GameState::GAME_OVER) {
        int w = GetScreenWidth();
        int h = GetScreenHeight();
        // overlay semitransparente
        DrawRectangle(0, 0, w, h, (Color){0,0,0,120});
        std::string winnerText = message.empty() ? "Jogo terminado" : message;
        // Centralizar texto grande
    Vector2 measure = MeasureTextEx(fonteTitulo, winnerText.c_str(), 32, 2);
    DrawTextEx(fonteTitulo, winnerText.c_str(), (Vector2){(((float)w - measure.x)/2.0f), (((float)h / 2.0f) - 40.0f)}, 32, 2, WHITE);
        const char *btn = "Pressione R para reiniciar";
        Vector2 m2 = MeasureTextEx(fonteTexto, btn, 20, 1);
        DrawRectangle((w - (int)m2.x - 20)/2, (h/2) + 10, (int)m2.x + 20, 40, (Color){200,200,200,255});
        DrawTextEx(fonteTexto, btn, (Vector2){(float)((w - (int)m2.x)/2), (float)((h/2) + 18)}, 20, 1, BLACK);
    }
    EndDrawing();
}

// Desenha as informações dos jogadores
void Game::renderPlayers() {
    //define a altura na posição vertical do painel
    int painelY = 2;
   // DrawRectangle(20, painelY, GetScreenWidth() - 40, 200, LIGHTGRAY); // x, y, width, height, color (LIGHTGRAY)
    
    // Jogador 1
    // Usa a cor configurada do jogador, mas suaviza para a UI para nao ficar muito intensa
    auto toneDown = [](Color c, float factor)->Color {
        Color out;
        out.r = (unsigned char)(c.r * factor + 255 * (1.0f - factor));
        out.g = (unsigned char)(c.g * factor + 255 * (1.0f - factor));
        out.b = (unsigned char)(c.b * factor + 255 * (1.0f - factor));
        out.a = c.a;
        return out;
    };
    auto pickTextColor = [](Color bg)->Color {
        int lum = (bg.r + bg.g + bg.b) / 3;
        return (lum < 140) ? WHITE : BLACK;
    };

    // Helper local para desenhar a carteira de um jogador como uma tabela compacta
    auto renderPortfolio = [&](int pid, int baseX, int baseY, Color textColor, int textSize){
        std::map<std::string,int> counts;
        for (const auto& prop : players[pid]->getProperties()) {
            if (auto s = std::dynamic_pointer_cast<Stock>(prop)) counts[s->getTicker()]++;
            else if (auto f = std::dynamic_pointer_cast<FII>(prop)) counts[f->getTicker()]++;
            else counts[prop->getName()]++;
        }
        if (pid < (int)extraHoldings.size()) {
            for (const auto &kv : extraHoldings[pid]) counts[kv.first] += kv.second;
        }

        // cabeçalho da tabela
        int colTickerW = 70;
        int colUnitsW = 40;
        int colAvgW = 60;
        int colPriceW = 60;
        int colTotalW = 70;
        int colPLW = 60;
    int rowH = textSize + 6; // altura da linha ajustada ao tamanho do texto
        int y = baseY;
        DrawTextEx(fonteNegrito, "Ativo", (Vector2){(float)baseX, (float)y}, textSize, 1, textColor);
        DrawTextEx(fonteNegrito, "Un", (Vector2){(float)(baseX + colTickerW), (float)y}, textSize, 1, textColor);
        DrawTextEx(fonteNegrito, "Avg", (Vector2){(float)(baseX + colTickerW + colUnitsW), (float)y}, textSize, 1, textColor);
        DrawTextEx(fonteNegrito, "Prc", (Vector2){(float)(baseX + colTickerW + colUnitsW + colAvgW), (float)y}, textSize, 1, textColor);
        DrawTextEx(fonteNegrito, "Val", (Vector2){(float)(baseX + colTickerW + colUnitsW + colAvgW + colPriceW), (float)y}, textSize, 1, textColor);
        DrawTextEx(fonteNegrito, "P/L%", (Vector2){(float)(baseX + colTickerW + colUnitsW + colAvgW + colPriceW + colTotalW), (float)y}, textSize, 1, textColor);
        y += rowH;

        // ordenar por valor de mercado (desc)
        std::vector<std::pair<std::string,int>> items(counts.begin(), counts.end());
        std::sort(items.begin(), items.end(), [&](const auto &a, const auto &b){
            float pa=0,pb=0;
            for (const auto &p : board.getProperties()){
                if (auto s = std::dynamic_pointer_cast<Stock>(p)) { if (s->getTicker()==a.first) { pa = s->getPrice(); break; } }
                else if (auto f = std::dynamic_pointer_cast<FII>(p)) { if (f->getTicker()==a.first) { pa = f->getPrice(); break; } }
                else { if (p->getName()==a.first) { pa = p->getPrice(); break; } }
            }
            for (const auto &p : board.getProperties()){
                if (auto s = std::dynamic_pointer_cast<Stock>(p)) { if (s->getTicker()==b.first) { pb = s->getPrice(); break; } }
                else if (auto f = std::dynamic_pointer_cast<FII>(p)) { if (f->getTicker()==b.first) { pb = f->getPrice(); break; } }
                else { if (p->getName()==b.first) { pb = p->getPrice(); break; } }
            }
            return pa * a.second > pb * b.second;
        });

        int lineIdx = 0;
        for (const auto &kv : items) {
            const std::string &ticker = kv.first;
            int units = kv.second;
            float currentPrice = 0.0f;
            for (const auto &p : board.getProperties()) {
                if (auto s = std::dynamic_pointer_cast<Stock>(p)) { if (s->getTicker() == ticker) { currentPrice = s->getPrice(); break; } }
                else if (auto f = std::dynamic_pointer_cast<FII>(p)) { if (f->getTicker() == ticker) { currentPrice = f->getPrice(); break; } }
                else { if (p->getName() == ticker) { currentPrice = p->getPrice(); break; } }
            }
            float totalCost = 0.0f;
            float avgPrice = 0.0f;
            auto itcb = costBasis[pid].find(ticker);
            if (itcb != costBasis[pid].end()) {
                totalCost = itcb->second.totalCost;
                if (itcb->second.units > 0) avgPrice = totalCost / (float)itcb->second.units;
            }
            float currentValue = currentPrice * units;
            float profit = currentValue - totalCost;
            float profitPct = (totalCost > 0.0f) ? (profit / totalCost * 100.0f) : 0.0f;

            Color plColor = (profit >= 0.0f) ? (Color){30,150,30,255} : (Color){200,40,40,255};

            int rowY = y + lineIdx * rowH;
            DrawTextEx(fontePequena, ticker.c_str(), (Vector2){(float)baseX, (float)rowY}, textSize, 1, textColor);
            DrawTextEx(fontePequena, TextFormat("%d", units), (Vector2){(float)(baseX + colTickerW), (float)rowY}, textSize, 1, textColor);
            DrawTextEx(fontePequena, TextFormat("$%d", (int)avgPrice), (Vector2){(float)(baseX + colTickerW + colUnitsW), (float)rowY}, textSize, 1, textColor);
            DrawTextEx(fontePequena, TextFormat("$%d", (int)currentPrice), (Vector2){(float)(baseX + colTickerW + colUnitsW + colAvgW), (float)rowY}, textSize, 1, textColor);
            DrawTextEx(fontePequena, TextFormat("$%d", (int)currentValue), (Vector2){(float)(baseX + colTickerW + colUnitsW + colAvgW + colPriceW), (float)rowY}, textSize, 1, textColor);
            DrawTextEx(fontePequena, TextFormat("%+d%%", (int)profitPct), (Vector2){(float)(baseX + colTickerW + colUnitsW + colAvgW + colPriceW + colTotalW), (float)rowY}, textSize, 1, plColor);

            lineIdx++;
            // limitar linhas para não invadir o painel principal
            if (lineIdx >= 8) break;
        }

        // resumo abaixo
        float ativos = calculatePlayerAssetsValue(pid);
        float total = calculatePlayerTotalAssets(pid);
        DrawTextEx(fontePequena, TextFormat("Ativos: $%.0f  Total: $%.0f", ativos, total), (Vector2){(float)baseX, (float)(y + (lineIdx+1) * rowH)}, 14, 1, textColor);
    };

    Color player1Color = players[0]->getColor();
    Color bgJogador1 = (currentPlayerIndex == 0) ? toneDown(player1Color, 0.55f) : LIGHTGRAY;
    Color textJogador1 = (currentPlayerIndex == 0) ? pickTextColor(bgJogador1) : DARKGRAY;
    // Posição x começa em 30, largura é metade da tela menos margens
    DrawRectangle(20, painelY + 10, (GetScreenWidth() /2 -50) , 400, bgJogador1); // x, y, width, height, color (bgJogador1)

    // Mostrar nome, posicao e dinheiro na mesma linha para economizar espaço
    DrawTextEx(fonteNegrito, TextFormat("JOGADOR 1  |  Pos: %d  |  $%.0f", players[0]->getPosition(), players[0]->getMoney()), (Vector2){50, (float)painelY + 42}, 18, 1, textJogador1);
    // Mostrar carteira do Jogador 1 (tabela compacta) — fonte maior para melhor leitura
    renderPortfolio(0, 50, painelY + 62, textJogador1, 16);
    
    // Jogador 2
    Color player2Color = players[1]->getColor();
    Color bgJogador2 = (currentPlayerIndex == 1) ? toneDown(player2Color, 0.55f) : LIGHTGRAY;
    Color textJogador2 = (currentPlayerIndex == 1) ? pickTextColor(bgJogador2) : DARKGRAY;
    int player2PanelX = GetScreenWidth() / 2 + 10;
    DrawRectangle(player2PanelX, painelY + 10, ( GetScreenWidth()  /2 -40), 400, bgJogador2); // x, y, width, height, color (bgJogador2)

    // align player2 text same offset as player1: player1 text is at x=50 and its panel starts at x=20 -> offset 30
    int player1TextX = 50;
    int player1PanelX = 20;
    int textOffset = player1TextX - player1PanelX; // 30
    int player2TextX = player2PanelX + textOffset;

    DrawTextEx(fonteNegrito, TextFormat("JOGADOR 2  |  Pos: %d  |  $%.0f", players[1]->getPosition(), players[1]->getMoney()), (Vector2){(float)player2TextX, (float)painelY + 42}, 18, 1, textJogador2);
    // Mostrar carteira do Jogador 2 (tabela compacta) — manter fonte maior
    renderPortfolio(1, player2TextX, painelY + 62, textJogador2, 16);
}
// Desenha o tabuleiro do jogo
void Game::renderBoard() {
    
    int tabuleiroY = 450;
    int tabuleiroHeight = 300; // increased overall board panel height to fit larger houses
    
    // mostrar taxa SELIC logo acima do tabuleiro e indicar quantas voltas já foram concluídas
    DrawTextEx(fonteNegrito, TextFormat("SELIC: %.2f%%  |  Voltas concluídas: %d/4", selicRate, globalLapsCompleted), (Vector2){40, (float)tabuleiroY - 28}, 18, 1, BLACK);
    DrawRectangle(20, tabuleiroY, GetScreenWidth() - 40, tabuleiroHeight + 60, LIGHTGRAY); // x, y, width, height, color (board background)
    
    int casasPorLinha = 10;
    int casaWidth = (GetScreenWidth() - 80) / casasPorLinha;
    int casaHeight = 70; // <-- configurable: increase this to make houses taller (was 60)
    // auxiliar para suavizar cores de elementos da UI
    auto toneDown = [](Color c, float factor)->Color {
        Color out;
        out.r = (unsigned char)(c.r * factor + 255 * (1.0f - factor));
        out.g = (unsigned char)(c.g * factor + 255 * (1.0f - factor));
        out.b = (unsigned char)(c.b * factor + 255 * (1.0f - factor));
        out.a = c.a;
        return out;
    };
    
    // LINHA DE CIMA (0-9)
    for (int i = 0; i < casasPorLinha; i++) {
        int x = 40 + i * casaWidth;
    int y = tabuleiroY + 30;
        
        Color corCasa = GRAY;
        std::string textoCasa = std::to_string(i);
        Color corTexto = BLACK;
        bool propriedadeComprada = false;
        int donoPropriedade = 0;
        
    // get property pointer once
    auto propPtr = board.getPropertyAtPosition(i);
    std::string tickerOrName;
        if (board.isEventPosition(i)) {
            corCasa = YELLOW;
            textoCasa = "EV";
            corTexto = BLACK;
        } else if (propPtr) {
            corCasa = propPtr->getColor();
            corTexto = WHITE;
            propriedadeComprada = (propPtr->getOwner() != 0);
            donoPropriedade = propPtr->getOwner();
            // Mostrar ticker (ou nome) e preço atual na casa
            if (auto s = std::dynamic_pointer_cast<Stock>(propPtr)) tickerOrName = s->getTicker();
            else if (auto f = std::dynamic_pointer_cast<FII>(propPtr)) tickerOrName = f->getTicker();
            else tickerOrName = propPtr->getName();
            
            // SE A PROPRIEDADE FOI COMPRADA, ESCURECER A COR
            if (propriedadeComprada) {
                // Escurecer a cor para mostrar que foi comprada
                corCasa = (Color){
                    (unsigned char)(corCasa.r * 0.7f),  // 70% mais escuro
                    (unsigned char)(corCasa.g * 0.7f),
                    (unsigned char)(corCasa.b * 0.7f),
                    corCasa.a
                };
                
                // Adicionar borda colorida do dono
                if (donoPropriedade >= 1 && donoPropriedade <= (int)players.size()) {
                    Color ownerColor = players[donoPropriedade - 1]->getColor();
                    DrawRectangleLines(x, y, casaWidth - 4, casaHeight, toneDown(ownerColor, 0.6f));
                }
            }
        }
        
        // Desenha casa do tabuleiro
    DrawRectangle(x, y, casaWidth - 4, casaHeight, corCasa); // x, y, width, height, color (house background)
        
        // Texto: ticker/nome no topo e preço atual na base da casa (ou número/EV se não houver propriedade)
    if (propPtr) {
        DrawTextEx(fontePequena, tickerOrName.c_str(), (Vector2){(float)x + 5, (float)y + 8}, 14, 1, corTexto);
        DrawTextEx(fontePequena, TextFormat("$%.0f", propPtr->getPrice()), (Vector2){(float)x + 5, (float)y + casaHeight - 22}, 14, 1, corTexto);
    } else {
        DrawTextEx(fontePequena, textoCasa.c_str(), (Vector2){(float)x + 5, (float)y + 26}, 14, 1, corTexto);
    }
        
        // Símbolo de propriedade comprada
        if (propriedadeComprada) {
                if (donoPropriedade >= 1 && donoPropriedade <= (int)players.size()) {
                Color ownerColor = players[donoPropriedade - 1]->getColor();
                Color soft = toneDown(ownerColor, 0.85f);
                DrawCircle(x + casaWidth - 15, y + casaHeight/4, 8, soft);
                DrawTextEx(fontePequena, (std::string("J") + std::to_string(donoPropriedade)).c_str(), (Vector2){(float)x + casaWidth - 22, (float)y + casaHeight/4 - 6}, 12, 1, WHITE);
            }
        }
        
        // Marcadores de jogadores (linha de cima)
        // Draw marker for every player present on this square so events/showing who landed is visible
        for (size_t pid = 0; pid < players.size(); ++pid) {
            if (players[pid]->getPosition() == i) {
                Color markerColor = players[pid]->getColor();
                Color soft = (Color){(unsigned char)(markerColor.r * 0.9f), (unsigned char)(markerColor.g * 0.9f), (unsigned char)(markerColor.b * 0.9f), markerColor.a};
                // offset players slightly so two players on same square do not overlap
                int offsetX = (int)((int)pid - (int)players.size()/2) * 10; 
                DrawCircle(x + casaWidth/2 + offsetX, y - 10, 9, soft);
                DrawTextEx(fontePequena, std::to_string(players[pid]->getId()).c_str(), (Vector2){(float)x + casaWidth/2 + offsetX - 4, (float)y - 16}, 20, 1, WHITE);
            }
        }
    }
    
    // LINHA DO MEIO (10-19)
    for (int i = 0; i < casasPorLinha; i++) {
        int posicao = i + 10;
        int x = 40 + i * casaWidth;
        int y = tabuleiroY + 30 + casaHeight + 10; // middle row under top row
        
        Color corCasa = GRAY;
        std::string textoCasa = std::to_string(posicao);
        Color corTexto = BLACK;
        bool propriedadeComprada = false;
        int donoPropriedade = 0;
        
    auto propPtr = board.getPropertyAtPosition(posicao);
    std::string tickerOrName;
    if (board.isEventPosition(posicao)) {
            corCasa = YELLOW;
            textoCasa = "EV";
            corTexto = BLACK;
        } else if (propPtr) {
            corCasa = propPtr->getColor();
            corTexto = WHITE;
            propriedadeComprada = (propPtr->getOwner() != 0);
            donoPropriedade = propPtr->getOwner();
            if (auto s = std::dynamic_pointer_cast<Stock>(propPtr)) tickerOrName = s->getTicker();
            else if (auto f = std::dynamic_pointer_cast<FII>(propPtr)) tickerOrName = f->getTicker();
            else tickerOrName = propPtr->getName();
            
            // SE A PROPRIEDADE FOI COMPRADA, ESCURECER A COR
            if (propriedadeComprada) {
                corCasa = (Color){
                    (unsigned char)(corCasa.r * 0.7f),
                    (unsigned char)(corCasa.g * 0.7f),
                    (unsigned char)(corCasa.b * 0.7f),
                    corCasa.a
                };
                
                if (donoPropriedade >= 1 && donoPropriedade <= (int)players.size()) {
                    Color ownerColor = players[donoPropriedade - 1]->getColor();
                    DrawRectangleLines(x, y, casaWidth - 4, casaHeight, toneDown(ownerColor, 0.6f));
                }
            }
        }
        
        // Desenha casa do tabuleiro
    DrawRectangle(x, y, casaWidth - 4, casaHeight, corCasa); // x, y, width, height, color (middle row house)
        
        // Texto da propriedade da linha de baixo (ou número/EV se não houver propriedade)
    if (propPtr) {
        DrawTextEx(fontePequena, tickerOrName.c_str(), (Vector2){(float)x + 5, (float)y + 8}, 14, 1, corTexto);
        DrawTextEx(fontePequena, TextFormat("$%.0f", propPtr->getPrice()), (Vector2){(float)x + 5, (float)y + casaHeight - 22}, 14, 1, corTexto);
    } else {
        DrawTextEx(fontePequena, textoCasa.c_str(), (Vector2){(float)x + 5, (float)y + 26}, 14, 1, corTexto);
    }
        
        // Símbolo de propriedade comprada
        if (propriedadeComprada) {
            if (donoPropriedade >= 1 && donoPropriedade <= (int)players.size()) {
                Color ownerColor = players[donoPropriedade - 1]->getColor();
                Color soft = toneDown(ownerColor, 0.85f);
                DrawCircle(x + casaWidth - 15, y + casaHeight/4, 8, soft);
                DrawTextEx(fontePequena, (std::string("J") + std::to_string(donoPropriedade)).c_str(), (Vector2){(float)x + casaWidth - 22, (float)y + casaHeight/4 - 6}, 12, 1, WHITE);
            }
        }
        
        // Marcadores de jogadores (linha do meio)
        for (size_t pid = 0; pid < players.size(); ++pid) {
            if (players[pid]->getPosition() == posicao) {
                Color markerColor = players[pid]->getColor();
                Color soft = (Color){(unsigned char)(markerColor.r * 0.9f), (unsigned char)(markerColor.g * 0.9f), (unsigned char)(markerColor.b * 0.9f), markerColor.a};
                int offsetX = (int)((int)pid - (int)players.size()/2) * 10;
                DrawCircle(x + casaWidth/2 + offsetX, y + casaHeight - 10, 9, soft);
                DrawTextEx(fontePequena, std::to_string(players[pid]->getId()).c_str(), (Vector2){(float)x + casaWidth/2 + offsetX - 4, (float)y + casaHeight - 16}, 20, 1, WHITE);
            }
        }
    }

    // LINHA DE BAIXO (20-29)
    for (int i = 0; i < casasPorLinha; i++) {
        int posicao = i + 20;
        int x = 40 + i * casaWidth;
    int y = tabuleiroY + 30 + (casaHeight + 10) * 2; // posiciona a linha de baixo abaixo da linha do meio

        Color corCasa = GRAY;
        std::string textoCasa = std::to_string(posicao);
        Color corTexto = BLACK;
        bool propriedadeComprada = false;
        int donoPropriedade = 0;
        
        auto propPtr = board.getPropertyAtPosition(posicao);
        std::string tickerOrName;
        if (board.isEventPosition(posicao)) {
            corCasa = YELLOW;
            textoCasa = "EV";
            corTexto = BLACK;
        } else if (propPtr) {
            corCasa = propPtr->getColor();
            corTexto = WHITE;
            propriedadeComprada = (propPtr->getOwner() != 0);
            donoPropriedade = propPtr->getOwner();
            if (auto s = std::dynamic_pointer_cast<Stock>(propPtr)) tickerOrName = s->getTicker();
            else if (auto f = std::dynamic_pointer_cast<FII>(propPtr)) tickerOrName = f->getTicker();
            else tickerOrName = propPtr->getName();
            
            // SE A PROPRIEDADE FOI COMPRADA, ESCURECER A COR
            if (propriedadeComprada) {
                corCasa = (Color){
                    (unsigned char)(corCasa.r * 0.7f),
                    (unsigned char)(corCasa.g * 0.7f),
                    (unsigned char)(corCasa.b * 0.7f),
                    corCasa.a
                };
                
                if (donoPropriedade >= 1 && donoPropriedade <= (int)players.size()) {
                    Color ownerColor = players[donoPropriedade - 1]->getColor();
                    DrawRectangleLines(x, y, casaWidth - 4, casaHeight, toneDown(ownerColor, 0.6f));
                }
            }
        }
        
        // Desenha casa do tabuleiro
        DrawRectangle(x, y, casaWidth - 4, casaHeight, corCasa); // x, y, width, height, color (bottom row house)
        
        if (propPtr) {
            DrawTextEx(fontePequena, tickerOrName.c_str(), (Vector2){(float)x + 5, (float)y + 8}, 14, 1, corTexto);
            DrawTextEx(fontePequena, TextFormat("$%.0f", propPtr->getPrice()), (Vector2){(float)x + 5, (float)y + casaHeight - 22}, 14, 1, corTexto);
        } else {
            DrawTextEx(fontePequena, textoCasa.c_str(), (Vector2){(float)x + 5, (float)y + 26}, 14, 1, corTexto);
        }
        
        if (propriedadeComprada) {
            if (donoPropriedade >= 1 && donoPropriedade <= (int)players.size()) {
                Color ownerColor = players[donoPropriedade - 1]->getColor();
                Color soft = toneDown(ownerColor, 0.85f);
                DrawCircle(x + casaWidth - 15, y + casaHeight/4, 8, soft);
                DrawTextEx(fontePequena, (std::string("J") + std::to_string(donoPropriedade)).c_str(), (Vector2){(float)x + casaWidth - 22, (float)y + casaHeight/4 - 6}, 12, 1, WHITE);
            }
        }
        
        // Marcadores de jogadores (linha de baixo)
        for (size_t pid = 0; pid < players.size(); ++pid) {
            if (players[pid]->getPosition() == posicao) {
                Color markerColor = players[pid]->getColor();
                Color soft = (Color){(unsigned char)(markerColor.r * 0.9f), (unsigned char)(markerColor.g * 0.9f), (unsigned char)(markerColor.b * 0.9f), markerColor.a};
                int offsetX = (int)((int)pid - (int)players.size()/2) * 10;
                DrawCircle(x + casaWidth/2 + offsetX, y + casaHeight - 10, 9, soft);
                DrawTextEx(fontePequena, std::to_string(players[pid]->getId()).c_str(), (Vector2){(float)x + casaWidth/2 + offsetX - 4, (float)y + casaHeight - 16}, 20, 1, WHITE);
            }
        }
    }
    
    // QUARTA LINHA (30-39)
    for (int i = 0; i < casasPorLinha; i++) {
        int posicao = i + 30;
        int x = 40 + i * casaWidth;
    int y = tabuleiroY + 30 + (casaHeight + 10) * 3; // posiciona a quarta linha abaixo da terceira

        Color corCasa = GRAY;
        std::string textoCasa = std::to_string(posicao);
        Color corTexto = BLACK;
        bool propriedadeComprada = false;
        int donoPropriedade = 0;
        
        auto propPtr = board.getPropertyAtPosition(posicao);
        std::string tickerOrName;
        if (board.isEventPosition(posicao)) {
            corCasa = YELLOW;
            textoCasa = "EV";
            corTexto = BLACK;
        } else if (propPtr) {
            corCasa = propPtr->getColor();
            corTexto = WHITE;
            propriedadeComprada = (propPtr->getOwner() != 0);
            donoPropriedade = propPtr->getOwner();
            if (auto s = std::dynamic_pointer_cast<Stock>(propPtr)) tickerOrName = s->getTicker();
            else if (auto f = std::dynamic_pointer_cast<FII>(propPtr)) tickerOrName = f->getTicker();
            else tickerOrName = propPtr->getName();
            
            // SE A PROPRIEDADE FOI COMPRADA, ESCURECER A COR
            if (propriedadeComprada) {
                corCasa = (Color){
                    (unsigned char)(corCasa.r * 0.7f),
                    (unsigned char)(corCasa.g * 0.7f),
                    (unsigned char)(corCasa.b * 0.7f),
                    corCasa.a
                };
                
                if (donoPropriedade >= 1 && donoPropriedade <= (int)players.size()) {
                    Color ownerColor = players[donoPropriedade - 1]->getColor();
                    DrawRectangleLines(x, y, casaWidth - 4, casaHeight, toneDown(ownerColor, 0.6f));
                }
            }
        }
        
        // Desenha casa do tabuleiro
        DrawRectangle(x, y, casaWidth - 4, casaHeight, corCasa); // x, y, width, height, color (fourth row house)
        
        if (propPtr) {
            DrawTextEx(fontePequena, tickerOrName.c_str(), (Vector2){(float)x + 5, (float)y + 8}, 14, 1, corTexto);
            DrawTextEx(fontePequena, TextFormat("$%.0f", propPtr->getPrice()), (Vector2){(float)x + 5, (float)y + casaHeight - 22}, 14, 1, corTexto);
        } else {
            DrawTextEx(fontePequena, textoCasa.c_str(), (Vector2){(float)x + 5, (float)y + 26}, 14, 1, corTexto);
        }
        
        if (propriedadeComprada) {
            if (donoPropriedade >= 1 && donoPropriedade <= (int)players.size()) {
                Color ownerColor = players[donoPropriedade - 1]->getColor();
                Color soft = toneDown(ownerColor, 0.85f);
                DrawCircle(x + casaWidth - 15, y + casaHeight/4, 8, soft);
                DrawTextEx(fontePequena, (std::string("J") + std::to_string(donoPropriedade)).c_str(), (Vector2){(float)x + casaWidth - 22, (float)y + casaHeight/4 - 6}, 12, 1, WHITE);
            }
        }
        
        // Marcadores de jogadores (linha de baixo)
        for (size_t pid = 0; pid < players.size(); ++pid) {
            if (players[pid]->getPosition() == posicao) {
                Color markerColor = players[pid]->getColor();
                Color soft = (Color){(unsigned char)(markerColor.r * 0.9f), (unsigned char)(markerColor.g * 0.9f), (unsigned char)(markerColor.b * 0.9f), markerColor.a};
                int offsetX = (int)((int)pid - (int)players.size()/2) * 10;
                DrawCircle(x + casaWidth/2 + offsetX, y + casaHeight - 10, 9, soft);
                DrawTextEx(fontePequena, std::to_string(players[pid]->getId()).c_str(), (Vector2){(float)x + casaWidth/2 + offsetX - 4, (float)y + casaHeight - 16}, 20, 1, WHITE);
            }
        }
    }
}

// Nota: Game::renderPropertiesList foi removido porque não é chamado em nenhum lugar do programa.

void Game::renderInstructions() {
    // Instruções no rodapé: única linha centrada próximo ao fundo da janela
    int instrucoesY = GetScreenHeight() - 50; // posiciona as instruções próximo à parte inferior
    // Azul mais suave para o fundo das instruções (barra fina)
    DrawRectangle(20, instrucoesY - 6, GetScreenWidth() - 40, 36, (Color){70, 130, 180, 255}); // x, y, width, height, color (instructions background)

    // Instrucoes em linha unica separadas por pipe
    const char *instr = "ESPACO = Rolar dado  |  C = Comprar propriedade  |  N = Passar a vez  |  I = Investir  V = Vender";
    DrawTextEx(fonteTexto, instr, (Vector2){40, (float)instrucoesY}, 16, 1, WHITE);
}

void Game::initializePlayers() {
    // Usa cores padrão mais suaves para os jogadores, tornando a UI mais agradável
    players.push_back(std::make_shared<Player>(1, "Jogador 1", (Color){210, 85, 95, 255}, 1500.0f)); // muted red
    players.push_back(std::make_shared<Player>(2, "Jogador 2", (Color){80, 140, 200, 255}, 1500.0f)); // muted blue
}

void Game::processInvestProperty() {
    auto currentPlayer = players[currentPlayerIndex];
    if (!currentProperty) {
        setMessage("Nenhuma propriedade selecionada para investir.");
        currentState = GameState::PLAYER_TURN;
        return;
    }

    // Investimento: paga 50% do preço atual para aumentar price e rent
    float investCost = currentProperty->getPrice() * 0.5f;
    if (currentPlayer->canAfford(investCost)) {
        currentPlayer->addMoney(-investCost);
        // Aplicar aumento -- usar updatePrice com percent
        currentProperty->updatePrice(10.0f); // +10%
        currentProperty->updateRent(5.0f);   // +5%
        setMessage(currentPlayer->getName() + " investiu em " + currentProperty->getName() + ".");
        // Registrar compra adicional como unidade extra do ticker
        std::string key;
        if (auto s = std::dynamic_pointer_cast<Stock>(currentProperty)) key = s->getTicker();
        else if (auto f = std::dynamic_pointer_cast<FII>(currentProperty)) key = f->getTicker();
        else key = currentProperty->getName();
    extraHoldings[currentPlayerIndex][key]++;
    // update cost basis for invested extra unit
    auto &cb = costBasis[currentPlayerIndex][key];
    cb.units += 1;
    cb.totalCost += investCost;
    } else {
        setMessage("Dinheiro insuficiente para investir.");
    }

    currentProperty.reset();
    currentState = GameState::PLAYER_TURN;
    // registrar patrimonio apos a acao para popular o grafico
    samplePatrimonyHistory();
    nextTurn();
}

void Game::processSellProperty() {
    auto currentPlayer = players[currentPlayerIndex];
    if (!currentProperty) {
        setMessage("Nenhuma propriedade selecionada para vender.");
        currentState = GameState::PLAYER_TURN;
        return;
    }

    // Calcula venda e ajusta o custo contábil para o ticker (aproximação por redução do custo médio)
    std::string key;
    if (auto s = std::dynamic_pointer_cast<Stock>(currentProperty)) key = s->getTicker();
    else if (auto f = std::dynamic_pointer_cast<FII>(currentProperty)) key = f->getTicker();
    else key = currentProperty->getName();

    auto &playerCBMap = costBasis[currentPlayerIndex];
    auto itcb = playerCBMap.find(key);
    if (itcb != playerCBMap.end() && itcb->second.units > 0) {
        // average cost per unit
        float avgCost = itcb->second.totalCost / (float)itcb->second.units;
        itcb->second.totalCost -= avgCost;
        itcb->second.units -= 1;
        if (itcb->second.units == 0) playerCBMap.erase(itcb);
    }

    // Vendendo volta o preço atual
    currentPlayer->sellProperty(currentProperty);
    setMessage(currentPlayer->getName() + " vendeu " + currentProperty->getName() + ".");

    // Se havia unidade extra registrada pelo ticker, decrementar
    auto &mapRef = extraHoldings[currentPlayerIndex];
    auto it = mapRef.find(key);
    if (it != mapRef.end()) {
        if (it->second > 0) it->second--; // decrementar unidade extra
        if (it->second == 0) mapRef.erase(it);
    }

    currentProperty.reset();
    currentState = GameState::PLAYER_TURN;
    // registrar patrimonio apos a venda para popular o grafico
    samplePatrimonyHistory();
    nextTurn();
}

float Game::calculatePlayerTotalAssets(int playerIndex) const {
    if (playerIndex < 0 || playerIndex >= (int)players.size()) return 0.0f;
    float total = players[playerIndex]->getMoney();
    // propriedades possuidas diretamente
    for (const auto& prop : players[playerIndex]->getProperties()) {
        total += prop->getPrice();
    }
    // unidades extras registradas (extraHoldings)
    if (playerIndex < (int)extraHoldings.size()) {
        for (const auto &kv : extraHoldings[playerIndex]) {
            // procurar propriedade correspondente no board para pegar o preço atual
            // tentamos achar por ticker ou nome
            float price = 0.0f;
            for (const auto &p : board.getProperties()) {
                if (auto s = std::dynamic_pointer_cast<Stock>(p)) {
                    if (s->getTicker() == kv.first) { price = s->getPrice(); break; }
                } else if (auto f = std::dynamic_pointer_cast<FII>(p)) {
                    if (f->getTicker() == kv.first) { price = f->getPrice(); break; }
                } else {
                    if (p->getName() == kv.first) { price = p->getPrice(); break; }
                }
            }
            total += price * kv.second;
        }
    }
    return total;
}

float Game::calculatePlayerAssetsValue(int playerIndex) const {
    if (playerIndex < 0 || playerIndex >= (int)players.size()) return 0.0f;
    float total = 0.0f;
    // propriedades possuidas diretamente (cada Property representa 1 unidade)
    for (const auto& prop : players[playerIndex]->getProperties()) {
        total += prop->getPrice();
    }
    // unidades extras registradas (extraHoldings) - procurar preço atual por ticker
    if (playerIndex < (int)extraHoldings.size()) {
        for (const auto &kv : extraHoldings[playerIndex]) {
            float price = 0.0f;
            for (const auto &p : board.getProperties()) {
                if (auto s = std::dynamic_pointer_cast<Stock>(p)) {
                    if (s->getTicker() == kv.first) { price = s->getPrice(); break; }
                } else if (auto f = std::dynamic_pointer_cast<FII>(p)) {
                    if (f->getTicker() == kv.first) { price = f->getPrice(); break; }
                } else {
                    if (p->getName() == kv.first) { price = p->getPrice(); break; }
                }
            }
            total += price * kv.second;
        }
    }
    return total;
}

// Define a mensagem destacada (persistente até ser sobrescrita)
void Game::setMessage(const std::string &msg, bool isEvent) {
    message = msg;
    isEventMessage = isEvent;
}

// Anexa texto à mensagem atual (persistente)
void Game::appendMessage(const std::string &more) {
    if (message.empty()) {
        message = more;
        // Se a mensagem estava vazia, considera como não-evento por padrão
        isEventMessage = false;
    } else {
        message += more;
    }
    // Mantém o estado atual de isEventMessage quando apenas anexa texto
}

// Renderiza um mini-gráfico de patrimônio por jogador usando `patrimonyHistory`.
void Game::renderPatrimonyChart() {
    // parâmetro do gráfico
    const int chartW = 320;
    const int chartH = 90;
    const int padding = 8;
    int chartX = GetScreenWidth() - 40 - chartW;
    int chartY = GetScreenHeight() - 50 - chartH - 12; // acima das instruções

    DrawRectangle(chartX, chartY, chartW, chartH, (Color){240,240,240,200});
    DrawRectangleLines(chartX, chartY, chartW, chartH, DARKGRAY);

    // janela de amostra (últimos N pontos)
    const int N = 60;
    // encontrar máximo entre todos os jogadores nas últimas N entradas
    float globalMax = 0.0f;
    for (size_t p = 0; p < patrimonyHistory.size(); ++p) {
        const auto &hist = patrimonyHistory[p];
        int start = std::max(0, (int)hist.size() - N);
        for (int i = start; i < (int)hist.size(); ++i) globalMax = std::max(globalMax, hist[i]);
    }
    if (globalMax <= 0.0f) globalMax = 1.0f; // evita divisão por zero

    // desenhar linhas por jogador
    for (size_t p = 0; p < patrimonyHistory.size(); ++p) {
        const auto &hist = patrimonyHistory[p];
        int count = (int)hist.size();
        if (count < 2) continue; // nada a desenhar
        int start = std::max(0, count - N);
        int points = count - start;
        if (points < 2) continue;

        Color col = (p < players.size()) ? players[p]->getColor() : BLUE;
        // desenhar linha
        for (int i = 0; i < points - 1; ++i) {
            float v0 = hist[start + i];
            float v1 = hist[start + i + 1];
            float x0 = chartX + padding + (float)i / (float)(N - 1) * (chartW - padding*2);
            float x1 = chartX + padding + (float)(i + 1) / (float)(N - 1) * (chartW - padding*2);
            float y0 = chartY + chartH - padding - (v0 / globalMax) * (chartH - padding*2);
            float y1 = chartY + chartH - padding - (v1 / globalMax) * (chartH - padding*2);
            Vector2 p0 = {(float)x0, (float)y0};
            Vector2 p1 = {(float)x1, (float)y1};
            DrawLineEx(p0, p1, 2.0f, col);
        }

        // legenda: pequeno quadrado e id
        int legendX = chartX + 6;
        int legendY = chartY + 6 + (int)p * 14;
        DrawRectangle(legendX, legendY, 10, 10, col);
        DrawTextEx(fontePequena, TextFormat("J%d", (int)p+1), (Vector2){(float)legendX + 14, (float)legendY - 2}, 12, 1, BLACK);
    }
}

void Game::restart() {
    // Resetar estado do tabuleiro e dos jogadores
    board = Board();
    for (auto &p : players) {
        p->reset();
    }
    // reset global state
    currentPlayerIndex = 0;
    lastDiceRoll = 0;
    message = "";
    waitingForPurchase = false;
    currentProperty.reset();
    roundsCompleted = 0;
    globalLapsCompleted = 0;
    currentState = GameState::PLAYER_TURN;
    // reset holdings/cost basis and patrimony history
    extraHoldings.clear();
    extraHoldings.resize(players.size());
    costBasis.clear();
    costBasis.resize(players.size());
    patrimonyHistory.clear();
    patrimonyHistory.resize(players.size());
    // reset SELIC to default
    selicRate = 13.75f;
}

void Game::samplePatrimonyHistory() {
    const size_t MAX_HISTORY = 120; // cap to avoid unbounded growth
    for (size_t i = 0; i < players.size(); ++i) {
        float assetsValue = calculatePlayerAssetsValue((int)i);
        patrimonyHistory[i].push_back(assetsValue);
        if (patrimonyHistory[i].size() > MAX_HISTORY) {
            size_t excess = patrimonyHistory[i].size() - MAX_HISTORY;
            patrimonyHistory[i].erase(patrimonyHistory[i].begin(), patrimonyHistory[i].begin() + excess);
        }
    }
}