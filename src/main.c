#include <SDL3/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "ChemistryEngineeringGame",
        800, 600,
        0
    );

    if (!window)
    {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
        }

        SDL_Delay(16); // ~60 FPS cap
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}