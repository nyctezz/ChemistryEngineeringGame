#include "application.h"


void application_init(_application* app)
{
    // window errors:
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        abort();
    }

    if (window_init(&app->window, "ChemistryEngineeringGame", 1600, 900, false) != 0)
    {
        printf("Window creation failed: %s\n", SDL_GetError());
        abort();
    }
    // ---

    // rendering errors:
    switch (renderer_init(&app->renderer, &app->window))
    {
        case 1:
            printf("Failed to create OpenGL context: %s\n", SDL_GetError());
            abort();
            break;

        case 2:
            printf("Failed to initialize GLAD\n");
            abort();
            break;

        case 3:
            printf("Failed to initialize renderer\n");
            abort();
            break;
    }
    // ---

    timer_init(&app->timer);
}

void application_process_events(_application* app)
{
    while (SDL_PollEvent(&app->event))
    {
        switch (app->event.type)
        {
            case SDL_EVENT_QUIT:
                app->is_running = false;
                break;

            case SDL_EVENT_WINDOW_RESIZED:
                int width  = app->event.window.data1;
                int height = app->event.window.data2;

                if (width > 0 && height > 0)
                {
                    app->window.width = width;
                    app->window.height = height;

                    update_viewport(&app->window);
                }

                break;

            default:
                break;
        }
    }
}

void application_run(_application* app)
{
    app->is_running = true;


    _world world;
    world_init(&world, 59, 194719281);


    _camera camera;
    camera_init(&camera, 0.0f, 0.0f, 5.0f, 20.0f, 4.0f);


    while (app->is_running)
    {
        application_process_events(app);

        timer_update(&app->timer);
        timer_print_fps(&app->timer);

        camera_update(&camera, app->timer.delta_time);
        renderer_run(&app->renderer, &app->window, &world, &camera);

        SDL_GL_SwapWindow(app->window.handle);
    }
}
    
void application_destroy(_application* app)
{
    //world_destroy(&world);

    window_destroy(&app->window);
    renderer_destroy(&app->renderer);
    SDL_Quit();
}