#ifndef HANDLE_EVENT_H
#define HANDLE_EVENT_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include <string>
#include "action.h"
#include "Game.h"

bool addPiece(int targetX, int targetY) {
    int currentType = game->chess->getCurrentPieceType();
    if (game->getPlayer(currentType) != 0)
        return false;
    if (game->chess->addPiece(targetX, targetY, currentType)) {
        if (Mix_PlayChannel(-1, drop, 0) == -1)
            return 1;
        if (game->checkResult(currentType) && client != NULL) {
            /*SDLNet_TCP_Send(client,"e",2);
            client = NULL;*/
        }
    }
    return false;
}

bool setPlayer(int mX, int mY) {
    int player, type;
    bool set = true;
    if (mX > 635 && mX < 655 && mY > 225 && mY < 265) {
        game->setPlayer(type = 1, (player = game->getPlayer(1) + 2) % 3);
    } else if (mX > 635 && mX < 655 && mY > 325 && mY < 365) {
        game->setPlayer(type = 2, (player = game->getPlayer(2) + 2) % 3);
    } else if (mX > 770 && mX < 790 && mY > 225 && mY < 265) {
        game->setPlayer(type = 1, (player = game->getPlayer(1) + 1) % 3);
    } else if (mX > 770 && mX < 790 && mY > 325 && mY < 365) {
        game->setPlayer(type = 2, (player = game->getPlayer(2) + 1) % 3);
    } else
        set = false;
    if (set) {
        if (player == 2 && game->getPlayer(3 - type) != 2) {
            if (!isNetworkOpen) {
                server = SDLNet_TCP_Open(&ip);
                if (!server) {
                    return false;
                } else
                    isNetworkOpen = true;
            }
        } else if (player != 2 && game->getPlayer(3 - type) != 2) {
            if (isNetworkOpen) {
                if (client) {
                    SDLNet_TCP_Send(client, "e", 2);
                    SDLNet_TCP_Close(client);
                }
                SDLNet_TCP_Close(server);
                isNetworkOpen = false;
                client = NULL;
            }
        }
    }
    return set;
}

bool handleEvent(SDL_Event &event) {
    //Watch if need quit
    if (event.type == SDL_QUIT ||
        (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        return true;

    //Game data update

    if (event.type == SDL_MOUSEMOTION)
        game->moveMouse(event.motion.x, event.motion.y);
    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int stage = game->getStage();
        int mX = event.button.x;
        int mY = event.button.y;
        switch (stage) {
            case 0:
                if (mX > 250 && mX < 510 && mY > 450 && mY < 525)
                    game->nextStage();
                break;
            case 1:
                if (mX > 640 && mX < 790 && mY > 470 && mY < 525)
                    game->restart();
                else if (!setPlayer(mX, mY)) {
                    int targetX = game->getTargetX();
                    int targetY = game->getTargetY();
                    if (targetX >= 0 && targetY >= 0)
                        return addPiece(targetX, targetY);
                }
                break;
            case 2:
                if (mX > 640 && mX < 790 && mY > 470 && mY < 525)
                    game->restart();
                else
                    setPlayer(mX, mY);
                break;
        }
    }
    return false;
}

#endif
