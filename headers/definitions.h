#include "SDL2/SDL_image.h"


typedef struct
{
    int x, y,w,h;
    int falling;
    char *name;
}character;

enum tela{
    initscreen,pause, game, gameOver

};
typedef struct 
{
    character man;
    character llamas;
    character cactus[100];
    character floor;
    

    character grass;
    character hitbox;
    character sky;
    character gameOverScreen;
    SDL_Renderer *renderer;
    SDL_Texture *llama;
    SDL_Texture *cactu;
    SDL_Texture *skyTexture;
    SDL_Texture *gameOverScreenTexture;
    enum tela telaAtual;
}gamestate;

enum postionsAndSizes
{
    llamaPositionX = 200,
    llamaPositionY = 192,
    llamaWidth = 68,
    llammaHeight = 68,

    cactusPositionX = 900,
    cactusPositionY = 170,
    cactusHeight = 100,
    cactusWidth = 95,
    
    floorCollision = 0,
    floorXAxisStart = 0,
    floorYAxisStart = 260,
    floorXAxisEnd = 1080,
    floorHeight = 2000,

    


    grassHeight = 60,
    
    hitboxPostionX = 0,
    hitboxPostionY = 260,
    hitboxWidth = 10,
    hitboxHeight = 10,

    gameOverScreenX = 230,
    gameOverScreenY = 170,
    gameOverScreenW = 640,
    gameOverScreenH = 57,

    skyPositionX = 0,
    skyPositionY = 0,
    skyWidth = 1080,
    skyHeight = 441,

};

