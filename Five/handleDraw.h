#pragma once

#include <ctime>
#include "action.h"
#include "Game.h"

void drawEnterButton(const int &mX, const int &mY) {
    SDL_Rect temp;
    temp.h = 75;
    temp.w = 300;
    if (mX > 250 && mX < 510 && mY > 450 && mY < 525) {
        temp.x = 0;
        temp.y = 75;
    } else {
        temp.x = 0;
        temp.y = 0;
    }
    apply_surface(250, 450, button, screen, &temp);
}

void drawRestartButton(const int &mX, const int &mY) {
    SDL_Rect temp;
    if (mX > 615 && mX < 765 && mY > 470 && mY < 525) {
        temp.x = 0;
        temp.y = 225;
    } else {
        temp.x = 0;
        temp.y = 150;
    }
    temp.h = 75;
    temp.w = 170;
    apply_surface(610, 460, button, screen, &temp);
}

void drawSettingBox(const int &mX, const int &mY) {
    SDL_Rect temp;
    if (mX > 610 && mX < 630 && mY > 200 && mY < 240) {
        temp.x = 233;
    } else {
        temp.x = 184;
    }
    temp.y = 160;
    temp.h = 50;
    temp.w = 20;
    apply_surface(625, 200, button, screen, &temp);

    if (mX > 610 && mX < 630 && mY > 300 && mY < 340) {
        temp.x = 233;
    } else {
        temp.x = 184;
    }
    temp.y = 160;
    apply_surface(625, 300, button, screen, &temp);

    if (mX > 745 && mX < 765 && mY > 200 && mY < 240) {
        temp.x = 256;
    } else {
        temp.x = 207;
    }
    temp.y = 160;
    apply_surface(765, 200, button, screen, &temp);

    if (mX > 745 && mX < 765 && mY > 300 && mY < 340) {
        temp.x = 256;
    } else {
        temp.x = 207;
    }
    temp.y = 160;
    apply_surface(765, 300, button, screen, &temp);

    temp.x = 183;
    temp.y = 235;
    temp.w = 100;
    temp.h = 40;
    apply_surface(655, 205, button, screen, &temp);
    apply_surface(655, 305, button, screen, &temp);

    apply_surface(640, 173, str_black, screen);
    switch (game->getPlayer(1)) {
        case 0:
            apply_surface(660, 213, str_player, screen);
            break;
        case 1:
            apply_surface(660, 213, str_innerAI, screen);
            break;
        case 2:
            apply_surface(660, 213, str_outerAI, screen);
            break;
    }
    apply_surface(640, 273, str_white, screen);
    switch (game->getPlayer(2)) {
        case 0:
            apply_surface(660, 313, str_player, screen);
            break;
        case 1:
            apply_surface(660, 313, str_innerAI, screen);
            break;
        case 2:
            apply_surface(660, 313, str_outerAI, screen);
            break;
    }
}

void drawFocusBox(const int &mX, const int &mY) {
    if (game->getPlayer(game->chess->getCurrentPieceType()) != 0)
        return;
    int targetX = game->getTargetX();
    int targetY = game->getTargetY();
    if (targetX >= 0 && targetY >= 0) {
        SDL_Rect temp;
        temp.x = 0;
        temp.y = 35;
        temp.h = temp.w = 35;
        apply_surface(targetX * 35 + 60, targetY * 35 + 37, piece, screen, &temp);
    }
}

void DrawPieces() {
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++) {
            int type = game->chess->hasPiece(i, j);
            if (!type)
                continue;
            SDL_Rect temp;
            temp.h = temp.w = 35;
            if (type == 1) {
                temp.x = 0;
                temp.y = 0;
            } else {
                temp.x = 35;
                temp.y = 0;
            }
            apply_surface(i * 35 + 60, j * 35 + 37, piece, screen, &temp);

        }
    int lastX, lastY;
    game->chess->getLastPiece(lastX, lastY);
    if (lastX >= 0 && lastY >= 0) {
        SDL_Rect temp;
        temp.h = temp.w = 35;
        temp.x = 35;
        temp.y = 35;
        apply_surface(lastX * 35 + 60, lastY * 35 + 37, piece, screen, &temp);
    }
}

void drawResult() {
    int winner = game->getWinner();
    if (winner == 1) {
        apply_surface(190, 230, blackWin, screen);
    } else if (winner == 2) {
        apply_surface(190, 230, whiteWin, screen);
    } else if (winner == 3) {
        apply_surface(190, 230, draw, screen);
    }
}

void handleDraw() {
    int mX = game->getMouseX() - 25, mY = game->getMouseY() - 25;
    switch (game->getStage()) {
        case 0:
            apply_surface(0, 0, welcome, screen);
            drawEnterButton(mX, mY);
            break;
        case 1:
            apply_surface(0, 0, table, screen);
            drawRestartButton(mX, mY);
            drawSettingBox(mX, mY);
            DrawPieces();
            drawFocusBox(mX, mY);
            break;
        case 2:
            apply_surface(0, 0, table, screen);
            drawRestartButton(mX, mY);
            drawSettingBox(mX, mY);
            DrawPieces();
            if (time(0) - game->getWinTime() < 2)
                drawResult();
    }
    apply_surface(mX, mY, hand, screen);
}
