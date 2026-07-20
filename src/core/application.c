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


    //_gameobject object1;

    
    // Each line represents a vertex: Position (X, Y, Z) and Texture Coordinate (U, V)
    /*float vertices[] = {
        // Positions          // Texture Coords (UV)
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom-left
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom-right
        0.0f,  0.5f, 0.0f,   0.5f, 1.0f  // Top-center
    };
    */
   
   float vertices[] = {
        // Positions            // Texture Coords (UV)
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,      0.0f, 1.0f,
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f
   };

    uint32_t indices[] = {
        0, 1, 2,
        1, 2, 3
    };
    
    _mesh mesh;

    mesh_init(&mesh, vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(uint32_t));
    
    uint32_t my_texture = texture_load_png("assets/textures/debug.png");


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
                update_viewport(&app->window);
                break;
        }

        glClearColor(0.15f, 0.20f, 0.30f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        renderer_run(&app->renderer);

        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, my_texture);

        mesh_draw(&mesh);

        SDL_GL_SwapWindow(app->window.handle);
    }
}
    
void application_destroy(_application* app)
{
    window_destroy(&app->window);
    renderer_destroy(&app->renderer);
    SDL_Quit();
}