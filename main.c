#include <stdio.h>
#include <SDL2/SDL.h>
#include "character.h"
#include <time.h>

/*Links:
http://www.lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
Compilar: gcc -o main main.c -lSDL2 -lSDL2_image



Falta: 
    Renderizador da tela
    pontos 
    movimento do cactus
*/

void loadGame(gamestate *gameState)
{

    SDL_Surface *llamaSurface = NULL;
    SDL_Surface *cactusSurface = NULL;
    SDL_Surface *mountainSurface = NULL;

    gameState->llamas.x = llamaPositionX;
    gameState->llamas.y = llamaPositionY;
    gameState->llamas.w = llamaWidth;
    gameState->llamas.h = llammaHeight;

    gameState->cactus.x = cactusPositionX;
    gameState->cactus.y = cactusPositionY;

    gameState->grass.x = floorXAxisStart;
    gameState->grass.y = floorYAxisStart;
    gameState->grass.w = floorXAxisEnd;
    gameState->grass.h = grassHeight;
    
    gameState->floor.x = floorXAxisStart;
    gameState->floor.y = floorYAxisStart;
    gameState->floor.w = floorXAxisEnd;
    gameState->floor.h = floorHeight;

    gameState->hitbox.x = hitboxPostionX;
    gameState->hitbox.y = hitboxPostionY;
    gameState->hitbox.w = hitboxWidth;
    gameState->hitbox.h = hitboxHeight;


    llamaSurface = IMG_Load("llama-main.png");
    cactusSurface = IMG_Load("cactus.png");

    
    gameState->llama = SDL_CreateTextureFromSurface(gameState->renderer, llamaSurface);
    SDL_FreeSurface(llamaSurface);

    gameState->cactu = SDL_CreateTextureFromSurface(gameState->renderer, cactusSurface);
    SDL_FreeSurface(cactusSurface);

   
}
/*void ScoreCounting(gamestate *gameState)
{
    If the position of the cactus is equal to the check then the score increases
    FILE *fp = fopen("score.txt", "w");

    int score = 0;
    if(gameState->cactus.x == gameState->hitbox.x){
        score += 100;
        fprintf(fp,"%d", score);
    }
}*/
int processEvents(SDL_Window *window, gamestate *gameState, int *start)
{
    int done = 0;
    SDL_Event event;
    

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
        {

            if (window)
            {
                SDL_DestroyWindow(window);
                window == NULL;
                done = 1;
            }
        }
        break;

        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {

            case SDLK_ESCAPE:
                
                done = 1;
                
                break;
            case SDLK_SPACE:
                
                break;
            }

        }
        break;

        case SDL_QUIT:
            done = 1;
            break;
        }

    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_LEFT])
    {

       gameState->llamas.x -= 5;
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        gameState->llamas.x +=5;
    }
    if (state[SDL_SCANCODE_UP])
    {
        gameState->llamas.y -= 14;
        //gameState->llamas.x += 5;
    }
    /*if (state[SDL_SCANCODE_DOWN])
    {
        gameState->llamas.y += 10;
    }*/
    return done;
}

void collisionDetect(gamestate *gameState)
{
   /*Here I need to reset the position of the cactus when it touches the llama so the right side of the llama
   and the left side of the cactus have to detect collision*/

    float llama_x = gameState->llamas.x, llama_y = gameState->llamas.y;
    float cactus_x = gameState->cactus.x, cactus_y = gameState->cactus.y;

    float resetCactus = gameState->hitbox.x - 100;
    
    /*if(llamaRight > cactusleft+150){
        gameState->cactus.x = cactusPositionX;
    }
    */
   //Resets cactus when it exits the screen
    
    if(cactus_x < resetCactus)
    {
        gameState->cactus.x = cactusPositionX;
    }else
    //creates collision with the floor
    if(llama_y >= floorYAxisStart - floorCollision)
    {
        gameState->llamas.y = 140;
    }
    if(cactus_x-70 < llama_y && cactus_x == llama_x)
        gameState->cactus.x = cactusPositionX;
    
   
}
void llamaJump(gamestate *gameState)
{
    float llama_y = gameState->llamas.y;
    float cactus_move = gameState->cactus.x;
    if (llama_y < 210 )
    {
        gameState->llamas.y += 3;
    }
    if(cactus_move > gameState->hitbox.x)
    {
        gameState->cactus.x -= 5;
    }
    if(cactus_move <= gameState->hitbox.x)
        gameState->cactus.x = 700;
}
void doRender(SDL_Renderer *renderer, gamestate *gameState)
{
    

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 180, 255, 255);
    SDL_RenderClear(renderer);
    


    SDL_Rect llamaTexture = {gameState->llamas.x,gameState->llamas.y ,gameState->llamas.h,gameState->llamas.w };
    SDL_RenderCopyEx(renderer, gameState->llama, NULL, &llamaTexture, 0, NULL, 0);

    SDL_Rect cactusTexture = {gameState->cactus.x, gameState->cactus.y, 200, 200};
    SDL_RenderCopyEx(renderer, gameState->cactu, NULL, &cactusTexture, 0, NULL, 0);

    SDL_SetRenderDrawColor(renderer, 180, 155,0 , 0);

    SDL_Rect rect = {gameState->floor.x, gameState->floor.y, gameState->floor.w, gameState->floor.h};
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
    SDL_Rect grassRect = {gameState->grass.x, gameState->grass.y, gameState->grass.w, gameState->grass.h};
    SDL_RenderFillRect(renderer, &grassRect);
    
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
    SDL_Rect hitboxRect = {gameState->hitbox.x, gameState->hitbox.y, gameState->hitbox.w, gameState->hitbox.h};
    SDL_RenderFillRect(renderer, &hitboxRect);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{

    int done = 0;
    int start = 0;
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    gamestate gameState;

    SDL_Init(SDL_INIT_VIDEO);
    srandom((int)time(NULL));
    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "Jack LLaman - Llaming Around!", // window title
        SDL_WINDOWPOS_UNDEFINED,         // initial x position
        SDL_WINDOWPOS_UNDEFINED,         // initial y position
        1080,                            // width, in pixels
        720,                            // height, in pixels
        SDL_WINDOW_OPENGL                // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL)
    {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gameState.renderer = renderer;
    loadGame(&gameState);
    doRender(renderer, &gameState);

    while (!done)
    {
        done = processEvents(window, &gameState, &start);
        collisionDetect(&gameState);
        //ScoreCounting(&gameState);
        doRender(renderer, &gameState);
        llamaJump(&gameState);
    }
/*sdl2 collision check*/
    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gameState.llama);
    SDL_DestroyTexture(gameState.cactu);


    // Clean up
    SDL_Quit();

    return 0;
}
