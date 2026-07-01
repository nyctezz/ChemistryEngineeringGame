#include "application.h"


void application_init(_application* app)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        abort();
    }

    if (window_init(&app->window, "ChemistryEngineeringGame", 800, 600, true) == false)
    {
        printf("Window creation failed: %s\n", SDL_GetError());
        abort();
    }
}

void application_run(_application* app)
{
    app->is_running = true;

    while (app->is_running)
    {
        SDL_PollEvent(&app->event);

        switch (app->event.type)
        {
            case SDL_EVENT_QUIT:
                app->is_running = false;
                break;

            case SDL_EVENT_WINDOW_RESIZED:
                app->window.width = app->event.window.data1;
                app->window.height = app->event.window.data2;
                break;
        }




       
        //SDL_Delay(16); // ~60 FPS cap
    }
}
    
void application_destroy(_application* app)
{
    window_destroy(&app->window);
    SDL_Quit();
}