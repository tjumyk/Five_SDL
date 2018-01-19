#ifdef MINGW
#include "SDL/SDL.h"
#else

#include "SDL2/SDL.h"

#endif

#include "SDL_mixer.h"
#include "res.h"
#include "timer.h"
#include "action.h"
#include "handleEvent.h"
#include "handleDraw.h"

int main(int argc, char *args[]) {
    bool quit = false;
    Timer fps;
    if (!init())
        return 1;
    if (!load_files())
        return 1;
    if (Mix_PlayMusic(music, -1) == -1) {
        return 1;
    }
    while (!quit) {
        if (game->getStage() == 1) {
            int currentType = game->chess->getCurrentPieceType();
            int currentPlayer = game->getPlayer(currentType);
            if (currentPlayer == 1) {
                game->chess->AIWork(currentType);
                if (game->checkResult(currentType)) {
                    /*	SDLNet_TCP_Send(client,"e",2);
                        client = NULL;*/
                }
            } else if (currentPlayer == 2) {
                outerAIWork(currentType);
            }
        }
        while (SDL_PollEvent(&event)) {
            if (handleEvent(event))
                quit = true;
        }
        handleDraw();
        SDL_UpdateWindowSurface(window);
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
    }
    Mix_HaltMusic();
    clean_up();
    return 0;
}
