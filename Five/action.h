#ifndef ACTION_H
#define ACTION_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include <string>
#include "res.h"

SDL_Surface *load_image(std::string path) {
    //The final optimized image
    SDL_Surface *optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    } else {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, screen->format, 0);
        if (optimizedSurface == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        SDL_SetColorKey(optimizedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF));

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip = NULL) {
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface(source, clip, destination, &offset);
}


bool load_files() {
    //Load the images
    welcome = load_image("res/pic/welcome.bmp");
    table = load_image("res/pic/table.bmp");
    piece = load_image("res/pic/piece.bmp");
    blackWin = load_image("res/pic/blackWin.bmp");
    whiteWin = load_image("res/pic/whiteWin.bmp");
    draw = load_image("res/pic/draw.bmp");
    button = load_image("res/pic/button.bmp");
    hand = load_image("res/pic/hand.bmp");

    //Open the font
    font = TTF_OpenFont("res/font/font.ttf", 19);

    //If there was a problem in loading the background
    if (welcome == NULL || table == NULL || piece == NULL || blackWin == NULL || whiteWin == NULL
        || draw == NULL || button == NULL || hand == NULL)
        return false;

    //If there was an error in loading the font
    if (font == NULL) {
        return false;
    }

    //Set Font Style
    TTF_SetFontStyle(font, TTF_STYLE_BOLD | TTF_STYLE_ITALIC);

    //The color of the font
    SDL_Color textColor = {215, 215, 215};
    //Load text-surfaces
    str_player = TTF_RenderText_Blended(font, "  Player", textColor);
    str_innerAI = TTF_RenderText_Blended(font, "Inner AI", textColor);
    str_outerAI = TTF_RenderText_Blended(font, "Outer AI", textColor);

    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    textColor.r = textColor.g = textColor.b = 0;
    str_black = TTF_RenderText_Blended(font, "Black Control", textColor);
    textColor.r = textColor.g = textColor.b = 255;
    str_white = TTF_RenderText_Blended(font, "White Control", textColor);

    //If there was an error in rendering the text
    if (str_player == NULL || str_innerAI == NULL || str_outerAI == NULL) {
        return 1;
    }

    //Load the music
    music = Mix_LoadMUS("res/sound/background.ogg");

    //If there was a problem loading the music
    if (music == NULL)
        return false;

    //Load the sound effects
    drop = Mix_LoadWAV("res/sound/drop.wav");
    bomb = Mix_LoadWAV("res/sound/bomb.wav");

    //If there was a problem loading the sound effects
    if ((drop == NULL) || (bomb == NULL))
        return false;

    //If everything loaded fine
    return true;
}


void clean_up() {
    //Delete game kernel
    delete game;
    //Free the surfaces
    SDL_FreeSurface(welcome);
    SDL_FreeSurface(table);
    SDL_FreeSurface(piece);
    SDL_FreeSurface(blackWin);
    SDL_FreeSurface(whiteWin);
    SDL_FreeSurface(draw);
    SDL_FreeSurface(button);
    SDL_FreeSurface(hand);
    SDL_FreeSurface(str_player);
    SDL_FreeSurface(str_innerAI);
    SDL_FreeSurface(str_outerAI);
    SDL_FreeSurface(str_black);
    SDL_FreeSurface(str_white);

    //Free the sound effects
    Mix_FreeChunk(drop);
    Mix_FreeChunk(bomb);

    //Free the music
    Mix_FreeMusic(music);

    //Close the font
    TTF_CloseFont(font);

    //Quit SDL_mixer
    Mix_CloseAudio();

    //Quit SDL_ttf
    TTF_Quit();

    //Close Network
    if (isNetworkOpen) {
        if (client) {
            SDLNet_TCP_Send(client, "e", 2);
            SDLNet_TCP_Close(client);
        }
        SDLNet_TCP_Close(server);
    }
    //Quit SDL_net
    SDLNet_Quit();

    //Quit SDL
    SDL_Quit();
}

bool init() {
    //Initialize all SDL subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return false;
    }

    //Set up the screen
    window = SDL_CreateWindow("Five", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    screen = SDL_GetWindowSurface(window);

    //If there was an error in setting up the screen
    if (screen == NULL) {
        return false;
    }

    //Initialize SDL_ttf
    if (TTF_Init() == -1) {
        return false;
    }

    //Initialize SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        return false;
    }

    //Initialize SDL_net
    if (SDLNet_Init() == -1) {
        return false;
    }

    // create a listening TCP socket on port 4700 (server)
    if (SDLNet_ResolveHost(&ip, NULL, 4700) == -1) {
        return false;
    }
    isNetworkOpen = false;

    //Initialize game kernel
    game = new Game;

    //Disable the original cursor
    SDL_ShowCursor(SDL_DISABLE);

    //If everything initialized fine
    return true;
}


void outerAIWork(int currentType) {
    char data[257] = {0};
    data[0] = currentType;
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            data[i * 15 + j + 1] = game->chess->hasPiece(i, j);
    if (!client)
        client = SDLNet_TCP_Accept(server);
    if (!client) {
        printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
    } else {
        int result = SDLNet_TCP_Send(client, data, 257);
        if (result == 257) {
            char msg[3] = {0};
            result = SDLNet_TCP_Recv(client, msg, 3);
            if (result > 0) {
                if (msg[0] == 'e')
                    client = NULL;
                else {
                    game->chess->addPiece(msg[0], msg[1], currentType);
                    if (game->checkResult(currentType)) {
                        /*SDLNet_TCP_Send(client,"e",2);
                        client = NULL;*/
                    }
                }
            }
        }
    }
}

#endif
