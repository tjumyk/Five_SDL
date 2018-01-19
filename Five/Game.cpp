#include "Game.h"


Game::Game() {
    stage = 0;
    mouseX = mouseY = 0;
    targetX = targetY = -1;
    winTime = winner = 0;
    //Player:
    //0-player; 1-inner AI; 2-outer AI
    whitePlayer = 0;
    blackPlayer = 1;
    chess = new Chess;
}


Game::~Game() {
    delete chess;
}

void Game::moveMouse(int mX, int mY) {
    mouseX = mX;
    mouseY = mY;
    int tempX = (mX - 72) / 35;
    int tempY = (mY - 61) / 35;
    if (tempX >= 0 && tempX < 15 && tempY >= 0 && tempY < 15) {
        targetX = tempX;
        targetY = tempY;
    } else {
        targetX = targetY = -1;
    }
}
