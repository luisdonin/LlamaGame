#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "headers/definitions.h"
#include "headers/screen.h"




/*
Grupo
José Luis Bastos Donin
Luiz Felipe Nagatani
Pedro Gabriel
*/

/*Links:

Compilar: gcc -o main main.c -lSDL2 -lSDL2_image


*/

void loadGame(gamestate *gameState, screen_texture *start)
{
    /*
    Função carrega todas as texturas do jogo, criamos as superficies (surfaces), depois definimos as
    posições da llama, grama, chão, hitbox (usado pra checar se o cactus chegou no final do mapa), também usamos
    para tela de Game Over.
    Depois descobrimos que não era necessário, pois podia definir direto na função doRender().
    */
    int i = 0;
    SDL_Surface *llamaSurface = NULL;
    SDL_Surface *cactusSurface = NULL;
    SDL_Surface *mountainSurface = NULL;
    SDL_Surface *gameOverScreenSurface = NULL;
    SDL_Surface *skySurface = NULL;
    SDL_Surface *startScreenSurface = NULL;
    SDL_Surface *pauseSurface = NULL;
    SDL_Surface *logoSurface = NULL;
    SDL_Surface *studioSurface = NULL;
    SDL_Surface *restartLlammaSurface = NULL;
    
    gameState->llamas.x = llamaPositionX;
    gameState->llamas.y = llamaPositionY;
    gameState->llamas.w = llamaWidth;
    gameState->llamas.h = llammaHeight;
    gameState->llamas.falling = 0;

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

    //Img_load() carrega as imagens png nas superficies criadas acima.
    llamaSurface = IMG_Load("images/llamma-main.png");
    cactusSurface = IMG_Load("images/cactus.png");
    gameOverScreenSurface = IMG_Load("images/LLAME-OVER.png");
    skySurface = IMG_Load("images/sky.png");
    startScreenSurface = IMG_Load("images/startscreen.png");
    pauseSurface = IMG_Load("images/pause.png");
    logoSurface = IMG_Load("images/logo.png");
    studioSurface = IMG_Load("images/studio.png");
    restartLlammaSurface = IMG_Load("images/gameoverllama.png");

    //Aqui é onde definimos a posição do cactus que são gerandos randômicamente na posição X.
    for (i = 0; i < 100; i++)
    {
        gameState->cactus[i].x = cactusPositionX + rand() % 500;
        gameState->cactus[i].y = cactusPositionY;
    }
    /*Aqui são carregadas as texturas definidas em definitions.h e screen.h, essas funções 
    transformam as superficies em texturas e usa alocação dinâmica para transforma-las em texturas.
    */
    gameState->llama = SDL_CreateTextureFromSurface(gameState->renderer, llamaSurface);
    SDL_FreeSurface(llamaSurface);

    gameState->cactu = SDL_CreateTextureFromSurface(gameState->renderer, cactusSurface);
    SDL_FreeSurface(cactusSurface);

    gameState->gameOverScreenTexture = SDL_CreateTextureFromSurface(gameState->renderer, gameOverScreenSurface);
    SDL_FreeSurface(gameOverScreenSurface);

    gameState->skyTexture = SDL_CreateTextureFromSurface(gameState->renderer, skySurface);
    SDL_FreeSurface(skySurface);

    start->screenTexture = SDL_CreateTextureFromSurface(gameState->renderer, startScreenSurface);
    SDL_FreeSurface(startScreenSurface);

    start->pauseTexture = SDL_CreateTextureFromSurface(gameState->renderer, pauseSurface);
    SDL_FreeSurface(pauseSurface);

    start->logoTexture = SDL_CreateTextureFromSurface(gameState->renderer, logoSurface);
    SDL_FreeSurface(logoSurface);

    start->studioTexture = SDL_CreateTextureFromSurface(gameState->renderer, studioSurface);
    SDL_FreeSurface(studioSurface);

    start->restartLlammaTexture = SDL_CreateTextureFromSurface(gameState->renderer, restartLlammaSurface);
    SDL_FreeSurface(restartLlammaSurface);
}
void ScoreCounting(gamestate *gameState, screen_texture *start)
{

    /*
    Aqui definimos o score e escrevemos em um arquivo (score.txt), usamos a função time*60 para que mostre em
    números inteiros. 
    */
    clock_t t;
    FILE *fp = fopen("score/score.txt", "w"); 
    if(gameState->telaAtual == game)
    {
        start->lastTime = clock()*60;
        int score = ((int)start->lastTime)/CLOCKS_PER_SEC;

        printf("\nscore: %d", score);
        fscanf(fp, "%d", &score);
        fprintf(fp,"\nscore: %d", score);
    }
    fclose(fp);
}
int processEvents(SDL_Window *window, gamestate *gameState, screen_texture *start)
{
    /*Nessa função definimos todos os eventos do jogo, registramos uso do teclado e definimos 
    as funções que cada tecla faz
    1-Espaço -> começa o jogo
    2- P -> pausa o jogo
    3- Esc -> fecha o jogo
    4- também são definidas as telas do jogo*/
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
                    if (gameState->telaAtual == initscreen)
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
                case SDLK_p: 
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

    /*Não definimos o pulo usando SDLK pois diminui a fluidez do pulo, no entando
    quando fazemos dessa forma, temos que criar uma solução para o limite do pulo
    a função "SDL_SCANCODE_UP" registra todo o tempo em que a tecla esta apertada
    entao criamos um limitador com a ajuda do professor.
    */
    if (state[SDL_SCANCODE_UP] && !gameState->llamas.falling)
    {
        printf("\npulo");
        gameState->llamas.y -= 20;
 
        if(gameState->llamas.y < 5 )
        {
            gameState->llamas.y = 10;
            gameState->llamas.falling = 1;
            
        }  
        
    }
    
    return done;
}

void collisionDetection(gamestate *gameState)
{

    /*Nesta função definimos a colisão do cactus com a llama
    em partes inspirado em um tutorial do youtube, porém não conseguimos adaptar 
    o código do tutorial, logo, fizemos na base de tentativas até funcionar*/
    for(int i = 0; i < 100; i++)
    {
        float llamah = llammaHeight, llamaw = llamaWidth;
        float llamax = gameState->llamas.x, llamay = gameState->llamas.y;
        float cactusx = gameState->cactus[i].x, cactusy =  gameState->cactus[i].y, cactusw =  gameState->cactus[i].w, cactush =  gameState->cactus[i].h;
        /*Caso haja intersecção entre o spride do cactus com a llama
        o estado do jogo muda para "game over".*/
        if(llamay + llamah > cactusy && llamax < cactusx + cactush)
        
        {
            if(llamax <= cactusx + cactusw && llamax+llamaw >= cactusx+cactusw)
            {
                if (gameState->telaAtual == game)
                {
                    gameState->telaAtual = gameOver;
                }
            }
        }
    }
}

void gameLogic(gamestate *gameState)
{
    float llama_y = gameState->llamas.y;
    float cactus_move = gameState->cactus[0].x;
    float speed = 2;
    float acceleration = 3;
    float clouds_move = gameState->sky.x;
                      
    /*Limitador do pulo, enquanto o eixo y da llama for menor que a altura do chão, a llama caí.
    O ponto 0 do eixo y é no topo da tela, então quanto maior o valor no eixo Y, mais para baixo 
    a llama desce.*/ 
    if (llama_y < 191)
    {
        
        gameState->llamas.y += 5; // "velocidade" da queda da llama
        
        
    }
    else
        gameState->llamas.falling = 0;

    if (cactus_move >= gameState->hitbox.x)
    {   
       /*Função que controla o movimento do cactus e a velocidade*/
        acceleration++;
        gameState->cactus[0].x -= speed * acceleration;
    }
    /*Condição de controle de renderização dos cactus, quando 
    o cactus chega na posição do hitbox é resetado e trocado por um novo cactus em um array
    de 99 elementos.*/
    if (cactus_move <= gameState->hitbox.x)
    {
        for (int i = 1; i < 100; i++)
        {
            gameState->cactus[i - 1].x = gameState->cactus[i].x;
            
        }
        // cria um cactus novo ao final do vetor
        gameState->cactus[99].x = cactusPositionX + rand() % 500;
        gameState->cactus[99].y = cactusPositionY;
        
    }
 
}

void doRender(SDL_Renderer *renderer, gamestate *gameState,screen_texture *start )
{
    /*Nessa função é onde renderizamos todos os sprites e retângulos na tela
    de acordo com o estado do jogo.*/
    SDL_RenderClear(renderer);
    if(gameState->telaAtual == initscreen || gameState->telaAtual == gameOver )
    {
         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);//define a cor do plano de fundo (preta)
    }else
    SDL_SetRenderDrawColor(renderer, 0, 180, 255, 255); //define a cor do plano de fundo (azul)
    SDL_RenderClear(renderer);
    if(gameState->telaAtual == game || gameState->telaAtual == pause)
    {
    /*Quando o jogo está no estado "game" e "pause" todas as texturas são renderizadas
    usando SDL_rect e SDL_RenderCopy (usado para as texturas definidas em loadgame())*/

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

    SDL_Rect pause_Over_Text= {50, 20, 150, 30};
    SDL_RenderCopyEx(renderer, start->pauseTexture, NULL, &pause_Over_Text, 0, NULL, 0);
    }

    if(gameState->telaAtual == gameOver)
    {
        
        SDL_Rect Game_Over_Text= {gameState->gameOverScreen.x, gameState->gameOverScreen.y, gameState->gameOverScreen.w, gameState->gameOverScreen.h};
        SDL_RenderCopyEx(renderer, gameState->gameOverScreenTexture, NULL, &Game_Over_Text, 0, NULL, 0);

        SDL_Rect game_over_llama_Text= {400, 250, 217, 210};
        SDL_RenderCopyEx(renderer, start->restartLlammaTexture, NULL, &game_over_llama_Text, 0, NULL, 0);
        
        SDL_Rect Start_Game_Text= {230, 480, 640, 40};
        SDL_RenderCopyEx(renderer, start->screenTexture, NULL, &Start_Game_Text, 0, NULL, 0);
        

    }else if(gameState->telaAtual == initscreen)
    {
        
        SDL_Rect Start_Game_Text= {250, 170, 640, 40};
        SDL_RenderCopyEx(renderer, start->screenTexture, NULL, &Start_Game_Text, 0, NULL, 0);

        SDL_Rect logo_Text= {450, 520, 148, 249};
        SDL_RenderCopyEx(renderer, start->logoTexture, NULL, &logo_Text, 0, NULL, 0);

         
        SDL_Rect studio_Text= {450, 480, 149, 30};
        SDL_RenderCopyEx(renderer, start->studioTexture, NULL, &studio_Text, 0, NULL, 0);
    }
    

    SDL_RenderPresent(renderer);
}
void doUpdate(gamestate *gameState, int *done, screen_texture *start)
{
    /*Essa função controla o estado principal do jogo e as funçoes de score, colisão e lógica*/
    
    if (gameState->telaAtual == game)
    {
        ScoreCounting(gameState, start);
        collisionDetection(gameState);
        gameLogic(gameState);
       
    }
}
int main(int argc, char *argv[])
{

    int done = 0;
    
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    gamestate gameState;
    gameState.telaAtual = initscreen;
    screen_texture start;
    

    SDL_Init(SDL_INIT_VIDEO);
    srandom((int)time(NULL));
    // Definição e criação da janela.
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
    loadGame(&gameState, &start);
    doRender(renderer, &gameState, &start);
    //laço principal do jogo
    while (!done)
    {
        done = processEvents(window, &gameState, &start);
        doUpdate(&gameState, &done, &start);
        doRender(renderer, &gameState, &start);
        
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
