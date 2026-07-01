#include "application.h"


void application_init(_application* app)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        abort();
    }

    if (window_init(&app->window, "ChemistryEngineeringGame", 800, 600) == false)
    {
        printf("Window creation failed: %s\n", SDL_GetError());
        abort();
    }
}

void application_run(_application* app)
{
    bool running = true;
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

        //SDL_Delay(16); // ~60 FPS cap
    }
}
    
void application_destroy(_application* app)
{
    window_destroy(&app->window);
    SDL_Quit();
}