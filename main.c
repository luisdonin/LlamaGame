#include <stdio.h>
#include <SDL2/SDL.h>
#include "character.h"
#include <time.h>

/*Links:

Compilar: gcc -o main main.c -lSDL2 -lSDL2_image



Falta:
    Renderizador da tela
    pontos
    movimento do cactus
*/

void loadGame(gamestate *gameState)
{

    int i = 0;
    SDL_Surface *llamaSurface = NULL;
    SDL_Surface *cactusSurface = NULL;
    SDL_Surface *mountainSurface = NULL;
    SDL_Surface *gameOverScreenSurface = NULL;
    SDL_Surface *skySurface = NULL;

    gameState->llamas.x = llamaPositionX;
    gameState->llamas.y = llamaPositionY;
    gameState->llamas.w = llamaWidth;
    gameState->llamas.h = llammaHeight;

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

    gameState->gameOverScreen.x = gameOverScreenX;
    gameState->gameOverScreen.y = gameOverScreenY;
    gameState->gameOverScreen.w = gameOverScreenW;
    gameState->gameOverScreen.h = gameOverScreenH;
    
    gameState->sky.x = skyPositionX;
    gameState->sky.y = skyPositionY;
    gameState->sky.w = skyWidth;
    gameState->sky.h = skyHeight;

    llamaSurface = IMG_Load("llamma-main.png");
    cactusSurface = IMG_Load("cactus.png");
    gameOverScreenSurface = IMG_Load("gameoverscreen.png");
    skySurface = IMG_Load("sky.png");
    for (i = 0; i < 100; i++)
    {
        gameState->cactus[i].x = cactusPositionX + rand() % 500;
        gameState->cactus[i].y = cactusPositionY;
    }

    gameState->llama = SDL_CreateTextureFromSurface(gameState->renderer, llamaSurface);
    SDL_FreeSurface(llamaSurface);

    gameState->cactu = SDL_CreateTextureFromSurface(gameState->renderer, cactusSurface);
    SDL_FreeSurface(cactusSurface);

    gameState->gameOverScreenTexture = SDL_CreateTextureFromSurface(gameState->renderer, gameOverScreenSurface);
    SDL_FreeSurface(gameOverScreenSurface);

    gameState->skyTexture = SDL_CreateTextureFromSurface(gameState->renderer, skySurface);
    SDL_FreeSurface(skySurface);
}
/*void ScoreCounting(gamestate *gameState)
{
    //If the position of the cactus is equal to the check then the score increases
    int score = 1;
    int i = 0;
    for(i = 0; i < 100; i++)
    {
        if (gameState->cactus[i].x < gameState->llamas.x)
        {
            score++;
            printf("Score: %d", score);
        }
        
    }

}*/
int processEvents(SDL_Window *window, gamestate *gameState)
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
                    if (gameState->telaAtual == pause)
                    {
                        gameState->telaAtual = game;
                        gameState->cactus[0].x = cactusPositionX;
                        return done;
                    }else 
                    if(gameState->telaAtual == gameOver)
                    {
                        gameState->telaAtual = game;
                        gameState->cactus[0].x = cactusPositionX;
                        return done;
                    }else 
                    
                    break;
                case SDLK_a: 
                    if(gameState->telaAtual == game)
                    {
                        gameState->telaAtual = pause;
                        return done;
                    }else
                    if(gameState->telaAtual == pause)
                    {
                        gameState->telaAtual = game;
                        return done;
                    }
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

    
    if (state[SDL_SCANCODE_UP])
    {
        printf("\npulo");
        gameState->llamas.y -= 15;

        
    }
    
    return done;
}

void collisionDetection(gamestate *gameState)
{
    for(int i = 0; i < 100; i++)
    {
        float llamah = llammaHeight, llamaw = llamaWidth;
        float llamax = gameState->llamas.x, llamay = gameState->llamas.y;
        float cactusx = gameState->cactus[i].x, cactusy =  gameState->cactus[i].y, cactusw =  gameState->cactus[i].w, cactush =  gameState->cactus[i].h;
        
        if(llamay + llamah > cactusy && llamax < cactusx + cactush)
        
        {
            if(llamax <= cactusx + cactusw && llamax+llamaw >= cactusx+cactusw)
            {
                printf("pause");
                if (gameState->telaAtual == game)
                {
                    gameState->telaAtual = gameOver;

                }
            }

        }
      
    }
    

}

void llamaJump(gamestate *gameState)
{
    float llama_y = gameState->llamas.y;
    float cactus_move = gameState->cactus[0].x;
    float speed = 5;
    
    // limite do  pulo
    if (llama_y < 192)
    {
        //printf("\nQueda");
        gameState->llamas.y += 5;
    }

    if (cactus_move >= gameState->hitbox.x)
    {   
       // printf("\ncactus se move");
        gameState->cactus[0].x -= speed;
    }

    if (cactus_move <= gameState->hitbox.x)
    {
        printf("\n cactus reseta");
        
        for (int i = 1; i < 100; i++)
            gameState->cactus[i - 1].x = gameState->cactus[i].x;

        // cria um cactus novo ao final do vetor
        gameState->cactus[99].x = cactusPositionX + rand() % 500;
        gameState->cactus[99].y = cactusPositionY;
    }

}

void doRender(SDL_Renderer *renderer, gamestate *gameState)
{
    
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 180, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect sky_texture = {gameState->sky.x, gameState->sky.y, gameState->sky.w, gameState->sky.h};
    SDL_RenderCopyEx(renderer, gameState->skyTexture, NULL, &sky_texture, 0, NULL, 0);

    SDL_Rect llamaTexture = {gameState->llamas.x, gameState->llamas.y, gameState->llamas.h, gameState->llamas.w};
    SDL_RenderCopyEx(renderer, gameState->llama, NULL, &llamaTexture, 0, NULL, 0);

    SDL_Rect cactusTexture = {gameState->cactus[0].x, gameState->cactus[0].y, cactusWidth, cactusHeight};
    SDL_RenderCopyEx(renderer, gameState->cactu, NULL, &cactusTexture, 0, NULL, 0);

    SDL_SetRenderDrawColor(renderer, 180, 155, 0, 0);
    SDL_Rect rect = {gameState->floor.x, gameState->floor.y, gameState->floor.w, gameState->floor.h};
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
    SDL_Rect grassRect = {gameState->grass.x, gameState->grass.y, gameState->grass.w, gameState->grass.h};
    SDL_RenderFillRect(renderer, &grassRect);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
    SDL_Rect hitboxRect = {gameState->hitbox.x, gameState->hitbox.y, gameState->hitbox.w, gameState->hitbox.h};
    SDL_RenderFillRect(renderer, &hitboxRect);

    if(gameState->telaAtual == gameOver)
    {
        SDL_Rect G_Over_Text= {gameState->gameOverScreen.x, gameState->gameOverScreen.y, gameState->gameOverScreen.w, gameState->gameOverScreen.h};
        SDL_RenderCopyEx(renderer, gameState->gameOverScreenTexture, NULL, &G_Over_Text, 0, NULL, 0);
    }
    

    SDL_RenderPresent(renderer);
}
void doUpdate(gamestate *gameState, int *done)
{
    if (gameState->telaAtual == game)
    {
        //ScoreCounting(gameState);
        collisionDetection(gameState);
        llamaJump(gameState);
    }
}
int main(int argc, char *argv[])
{

    int done = 0;
    int start = 0;
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    gamestate gameState;
    gameState.telaAtual = pause;
    

    SDL_Init(SDL_INIT_VIDEO);
    srandom((int)time(NULL));
    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "Jack LLaman - Llaming Around!", // window title
        SDL_WINDOWPOS_UNDEFINED,         // initial x position
        SDL_WINDOWPOS_UNDEFINED,         // initial y position
        1080,                            // width, in pixels
        720,                             // height, in pixels
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
        done = processEvents(window, &gameState);
        doUpdate(&gameState, &done);
        doRender(renderer, &gameState);
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
