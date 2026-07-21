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
}

void application_run(_application* app)
{
    app->is_running = true;
   
    _camera camera;
    camera_init(&camera);

    _world world;
    world_init(&world, 5, 5);
    world_generate(&world);


    _mesh obj;
    mesh_init_quad(&obj);
    uint32_t my_texture = texture_load_png("assets/textures/dude.png");


    uint64_t previous_time = SDL_GetTicks();

    while (app->is_running)
    {
        uint64_t current_time = SDL_GetTicks();
        float delta_time = (float)(current_time - previous_time) / 1000.0f;
        previous_time = current_time;


        SDL_PollEvent(&app->event);

        switch (app->event.type)
        {
            case SDL_EVENT_QUIT:
                app->is_running = false;
                break;

            case SDL_EVENT_WINDOW_RESIZED:
                app->window.width = app->event.window.data1;
                app->window.height = app->event.window.data2;
                update_viewport(&app->window);
                break;
        }
        
        //glClearColor(0.15f, 0.20f, 0.30f, 1.0f);
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        camera_update(&camera, delta_time);

        renderer_run(&app->renderer, &world, &camera);

        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, my_texture);

        //mesh_draw(&obj);

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