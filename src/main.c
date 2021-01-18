#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

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

    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
           cells[x][y] = false;
        }
    }
    

    SDL_Event events;
    SDL_Rect rect;
 
    rect.w = CELLSIZE;
    rect.h = CELLSIZE;
    rect.x = 0;
    rect.y = 0;
    bool isOpen = true;
    bool isPlaying = false;
    int mouseX = 0;
    int mouseY = 0;
    int xIndex;
    int yIndex;
    
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
                    SDL_GetMouseState(&mouseX, &mouseY);
                    xIndex =  mouseX / CELLSIZE;
                    yIndex = mouseY / CELLSIZE;
                    bool *cell = &cells[xIndex][yIndex];
                    *cell = !*cell;
                case SDL_KEYDOWN:
                    if(events.key.keysym.sym == SDLK_SPACE)
                    {
                        isPlaying = !isPlaying;
                    }
                default:
                    break;
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
        }

        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);
        SDL_RenderClear(pRenderer);

        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {

                int color = cells[x][y] ? 0 : 255;

                rect.x = x * rect.w;
                rect.y = y * rect.h;

                SDL_SetRenderDrawColor(pRenderer, color, color, color, 0);
                SDL_RenderFillRect(pRenderer, &rect);

            }
            
        }
        
        SDL_RenderPresent(pRenderer);
    }

    status = EXIT_SUCCESS;
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);

Quit:
    SDL_Quit();
    return status;
}