#include "SDL2/SDL_image.h"
typedef struct
{
    int x, y,w,h;
    char *name;
}character;


typedef struct 
{
    character man;
    character llamas;
    character cactus[100];
    character floor;
    character grass;
    character hitbox;
    SDL_Renderer *renderer;
    SDL_Texture *llama;
    SDL_Texture *cactu;    
    
    
}gamestate;

enum postionsAndSizes
{
    llamaPositionX = 200,
    llamaPositionY = 200,
    llamaWidth = 200,
    llammaHeight = 200,

    cactusPositionX = 700,
    cactusPositionY = 140,
    
    floorCollision = 120,
    floorXAxisStart = 0,
    floorYAxisStart = 260,
    floorXAxisEnd = 2000,
    floorHeight = 2000,

    grassHeight = 60,
    
    hitboxPostionX = 10,
    hitboxPostionY = 260,
    hitboxWidth = 10,
    hitboxHeight = 10,

   

};

#define GRAVITY = 1