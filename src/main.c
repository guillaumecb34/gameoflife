#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

const int HEIGHT = 50;
const int WIDTH = 50;

const int CELLSIZE = 15;

int getAliveNeightboor(bool grid[WIDTH][HEIGHT], int x, int y)
{
    int aliveCount = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0) continue;

            if (x + i < 0 || y + j < 0) continue;

            if (x + i > WIDTH - 1 || y + j > HEIGHT - 1) continue;

            int neightboor = grid[x + i][y + j];

            aliveCount += neightboor;
        }
    }

    return aliveCount;
}

void renderText(SDL_Renderer *pRenderer, SDL_Surface *surface,
                SDL_Texture *texture, SDL_Rect dstrect, TTF_Font *font, SDL_Color textColor,
                char *msg, int positionX, int positionY)
{

    surface = TTF_RenderText_Solid(font, msg , textColor);
    texture = SDL_CreateTextureFromSurface(pRenderer, surface);
    int texW = 0;
    int texH = 0;

    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    int xtex;
    int ytex;

    dstrect.x = (positionX - surface->w/2);
    dstrect.y = (positionY - surface->h/2);
    dstrect.w = texW;
    dstrect.h = texH;

    SDL_RenderCopy(pRenderer, texture, NULL, &dstrect);
}

int main(int argc, char *argv[])
{
    SDL_Window *pWindow = NULL;
    SDL_Renderer *pRenderer = NULL;
    int status = EXIT_FAILURE;

    bool cells[WIDTH][HEIGHT];
    bool tempCells[WIDTH][HEIGHT];

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }

    

    pWindow = SDL_CreateWindow(
        "Game of Life",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH * CELLSIZE,
        HEIGHT * CELLSIZE,
        SDL_WINDOW_SHOWN
    );

    if(pWindow == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }

    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

    if(pRenderer == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }

    if(TTF_Init() != 0)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        goto QuitTTF;
    }

    if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) != 0)
    {
        printf("%s", Mix_GetError());
        goto QuitMix;
    }

    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
           cells[x][y] = false;
        }
    }
    

    SDL_Event events;
    SDL_Rect rect;
    SDL_Rect dstrect;
 
    rect.w = CELLSIZE;
    rect.h = CELLSIZE;
    rect.x = 0;
    rect.y = 0;
    bool isOpen = true;
    bool isStartPage = true;
    bool isPlaying = false;
    bool isGenerateGrid = false;
    int mouseX = 0;
    int mouseY = 0;
    int xIndex;
    int yIndex;

    int fontSize = 40;
    TTF_Font *font = TTF_OpenFont("Montserrat-Black.ttf", fontSize);
    SDL_Color textColor = { 0, 0, 0 };
    SDL_Surface *surface;
    SDL_Texture *texture;

    SDL_Surface *background;
    SDL_Texture *easterEgg;


    Mix_Music *music;
    music = Mix_LoadMUS("tetris.wav");
    Mix_PlayMusic(music, -1);

    bool r = false;
    bool a = false;
    bool f = false;
    bool a2 = false;
    bool l = false;
    bool e = false;
    
    bool w = false;
    bool t = false;
    bool f2 = false;

    int rAliveColor = 0;
    int gAliveColor = 0;
    int bAliveColor = 0;
    int rDeadColor = 255;
    int gDeadColor = 255;
    int bDeadColor = 255;

    while (isOpen)
    {
        if(SDL_PollEvent(&events))
        {
            switch (events.type)
            {
                case SDL_QUIT:
                    isOpen = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (isStartPage || isPlaying) break; 
                    
                    SDL_GetMouseState(&mouseX, &mouseY);
                    xIndex =  mouseX / CELLSIZE;
                    yIndex = mouseY / CELLSIZE;
                    bool *cell = &cells[xIndex][yIndex];
                    *cell = !*cell;
                    
                case SDL_KEYDOWN:
                    if(events.key.keysym.sym == SDLK_SPACE && isStartPage != true)
                    {
                        isPlaying = !isPlaying;
                    }
                    else if (events.key.keysym.sym == SDLK_RETURN && isStartPage != false)
                    {
                        isStartPage = !isStartPage;
                        isGenerateGrid = !isGenerateGrid;
                    }
                    else if (events.key.keysym.sym == SDLK_ESCAPE)
                    {
                        if (isStartPage != true)
                        {
                            isStartPage = !isStartPage;
                            isGenerateGrid = !isGenerateGrid;
                            if(isPlaying) isPlaying = !isPlaying;
                        }
                        else if (!r && !a && !f && !a2 && !l && !e)
                        {
                            isOpen = false;
                        }
                        
                        r = false;
                        a = false;
                        f = false;
                        a2 = false;
                        l = false;
                        e = false;
                    }
                    else if (events.key.keysym.sym == SDLK_p)
                    {
                        if(Mix_PausedMusic() == 1)
                        {
                            Mix_ResumeMusic();
                        }
                        else
                        {
                            Mix_PauseMusic();
                        }
                    }
                    else if (events.key.keysym.sym == SDLK_r)
                    {
                        if(isPlaying) break;
                        else if (isStartPage)
                        {
                            if(r) break;
                            r = true;
                        } 
                        else
                        {
                            for (int x = 0; x < WIDTH; x++)
                            {
                                for (int y = 0; y < HEIGHT; y++)
                                {
                                    cells[x][y] = rand() & 1;
                                }
                            }
                        }
                    }
                    else if (events.key.keysym.sym == SDLK_c && !isStartPage && !isPlaying)
                    {
                    
                        for (int x = 0; x < WIDTH; x++)
                        {
                            for (int y = 0; y < HEIGHT; y++)
                            {
                                cells[x][y] = false;
                            }
                        }
                    }
                    else if (events.key.keysym.sym == SDLK_m && !isStartPage)
                    {
                        rAliveColor = rand() % 256;
                        gAliveColor = rand() % 256;
                        bAliveColor = rand() % 256;
                        rDeadColor = rand() % 256;
                        gDeadColor = rand() % 256;
                        bDeadColor = rand() % 256;

                        if (w || t || f2)
                        {
                            w = false;
                            t = false;
                            f2 = false;
                        }

                    }
                    else if (events.key.keysym.sym == SDLK_a && isStartPage)
                    {
                        if(!a) a = true;
                        else if (a && !a2) a2 = true;
                        else break;
                    }
                    else if (events.key.keysym.sym == SDLK_f )
                    {
                        if(isStartPage && !f) 
                        {
                            f = true;
                        }
                        else if (!isStartPage && !f2)
                        {
                            f2 = true;
                        }
                        
                    }
                    else if (events.key.keysym.sym == SDLK_l && isStartPage && !l)
                    {
                        l = true;
                    }
                    else if (events.key.keysym.sym == SDLK_e && isStartPage && !e)
                    {
                        e = true;
                    }
                    else if (events.key.keysym.sym == SDLK_w && !isStartPage && !w)
                    {
                        w = true;
                    }
                    else if (events.key.keysym.sym == SDLK_t && !isStartPage && !t)
                    {
                        t = true;
                    }
                default:
                    break;
            }
        }
        

        if(isStartPage)
        {
            if(r && a && f && a2 && l && e)
            {
                
                SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);
                SDL_RenderClear(pRenderer);

                background = SDL_LoadBMP("easterEgg.bmp");

                easterEgg = SDL_CreateTextureFromSurface(pRenderer, background);

                SDL_RenderCopy(pRenderer, easterEgg, NULL, NULL);


            } 
            else
            {
                SDL_SetRenderDrawColor(pRenderer, 131, 3, 3, 0);
                SDL_RenderClear(pRenderer);
                renderText(pRenderer, surface, texture, dstrect, font, textColor, "GAME OF LIFE", ((CELLSIZE * WIDTH) / 2), ((CELLSIZE * HEIGHT - 150) / 2));
                renderText(pRenderer, surface, texture, dstrect, font, textColor, "Press Enter to Start", ((CELLSIZE * WIDTH) / 2), ((CELLSIZE * HEIGHT) / 2 + 100));
            }
            
        }
        
        
        if(isPlaying)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                for (int y = 0; y < HEIGHT; y++)
                {
                    tempCells[x][y] = cells[x][y];
                }
            }
            
            for (int x = 0; x < WIDTH; x++)
            {
                for (int y = 0; y < HEIGHT; y++)
                {
                  int aliveCount = getAliveNeightboor(tempCells, x, y);

                  if (aliveCount < 2 || aliveCount > 3 ) cells[x][y] = false;
                  else if (aliveCount == 3) cells[x][y] = true;  
                }   
            }
             SDL_Delay(100);
        }

        if(isGenerateGrid && isStartPage != true){
            SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);
            SDL_RenderClear(pRenderer);
           if (w && t && f2 && isPlaying) 
            {
                rAliveColor = rand() % 256;
                gAliveColor = rand() % 256;
                bAliveColor = rand() % 256;
                rDeadColor = rand() % 256;
                gDeadColor = rand() % 256;
                bDeadColor = rand() % 256;
            }
            for (int x = 0; x < WIDTH; x++)
            {
                for (int y = 0; y < HEIGHT; y++)
                {
                    
                    int rColor = cells[x][y] ? rAliveColor : rDeadColor;
                    int gColor = cells[x][y] ? gAliveColor : gDeadColor;
                    int bColor = cells[x][y] ? bAliveColor : bDeadColor;

                    

                    rect.x = x * rect.w;
                    rect.y = y * rect.h;

                    SDL_SetRenderDrawColor(pRenderer, rColor, gColor, bColor, 0);

                    SDL_RenderFillRect(pRenderer, &rect);

                }
                
            }
            
        }
            SDL_RenderPresent(pRenderer);
       
       
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    Mix_FreeMusic(music);

    status = EXIT_SUCCESS;
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);

QuitMix:
    Mix_CloseAudio();
QuitTTF:
    TTF_Quit();
Quit:
    SDL_Quit();
    return status;
}