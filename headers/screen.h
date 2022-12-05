#include <SDL2/SDL.h>
#include <time.h>

typedef struct 
{
    int x, y,w,h;
}screen_struct;

typedef struct 
{
    screen_struct screen;
    SDL_Texture *screenTexture;
    SDL_Texture *pauseTexture;
    SDL_Texture *logoTexture;
    SDL_Texture *studioTexture;
    SDL_Texture *restartLlammaTexture;
    time_t lastTime;
}screen_texture;

