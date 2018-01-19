#ifndef RES_H
#define RES_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

//The frame rate
const int FRAMES_PER_SECOND = 200;

//The event structure
SDL_Event event;

SDL_Window *window = NULL;
SDL_Surface *screen = NULL;

#include "Game.h"

//The surfaces
SDL_Surface *welcome = NULL;
SDL_Surface *table = NULL;
SDL_Surface *piece = NULL;
SDL_Surface *blackWin = NULL;
SDL_Surface *whiteWin = NULL;
SDL_Surface *draw = NULL;
SDL_Surface *button = NULL;
SDL_Surface *hand = NULL;
SDL_Surface *str_player = NULL;
SDL_Surface *str_innerAI = NULL;
SDL_Surface *str_outerAI = NULL;
SDL_Surface *str_black = NULL;
SDL_Surface *str_white = NULL;

//The font that will be used
TTF_Font *font;

//The music that will be played
Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *drop = NULL;
Mix_Chunk *bomb = NULL;

//SDL_net things
bool isNetworkOpen;
IPaddress ip;
TCPsocket server = NULL;
TCPsocket client = NULL;

//Game kernel
Game *game = NULL;

#endif
