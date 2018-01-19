#pragma once

#include <ctime>
#include "chess.h"

class Game {
public:
    Game();

    ~Game();

    int getStage() { return stage; }

    void nextStage() { if (stage < 2) stage++; }

    void moveMouse(int x, int y);

    int getMouseX() { return mouseX; }

    int getMouseY() { return mouseY; }

    int getTargetX() { return targetX; }

    int getTargetY() { return targetY; }

    void restart() {
        chess->resetPiece();
        stage = 1;
    }

    void setWinner(int win) {
        winner = win;
        if (winner != 0) {
            stage = 2;
            winTime = time(0);
        }
    }

    int getWinner() { return winner; }

    time_t getWinTime() { return winTime; }

    void setPlayer(int type, int player) {
        if (type == 1)
            blackPlayer = player;
        else if (type == 2)
            whitePlayer = player;
    }

    int getPlayer(int type) {
        if (type == 1)
            return blackPlayer;
        else if (type == 2)
            return whitePlayer;
        else
            return -1;
    }

    bool checkResult(int currentType) {
        bool hasResult = true;
        if (chess->checkFive(currentType))
            setWinner(currentType);
        else if (chess->checkFull())
            setWinner(3);
        else
            hasResult = false;
        return hasResult;
    }

    Chess *chess;
private:
    int stage;
    int mouseX, mouseY;
    int targetX, targetY;
    int winner;
    time_t winTime;
    int blackPlayer, whitePlayer;
};

