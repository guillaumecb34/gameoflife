#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Impossible d'initialiser la SDL.\n");
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Game of Life",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
    );
}