#include "application.h"


void application_init(_application* app)
{
    // window errors:
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        abort();
    }

    if (window_init(&app->window, "ChemistryEngineeringGame", 800, 600, false) == 1)
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
            {
                int width  = app->event.window.data1;
                int height = app->event.window.data2;

                if (width > 0 && height > 0)
                {
                    app->window.width = width;
                    app->window.height = height;

                    update_viewport(&app->window);
                }

                break;
            }

            default:
                break;
        }
    }
}

void application_run(_application* app)
{
    app->is_running = true;

    _world world;
    world_init(&world, 5, 5);
    world_generate(&world);


    _camera camera;
    camera_init(&camera, 0.0f, 0.0f, 5.0f, 4.0f);


    _mesh obj;
    mesh_init_quad(&obj);
    uint32_t my_texture = texture_load_png("assets/textures/dude.png");


    while (app->is_running)
    {
        app->timer.delta_time = timer_get_delta_time(&app->timer);

        application_process_events(app);
        camera_update(&camera, app->timer.delta_time);
        renderer_run(&app->renderer, &world, &camera, &app->window);

        // temporarily draw mesh manually, later wrap it in gameobject and renderer
        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, my_texture);
        mesh_draw(&obj);
        // ---

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