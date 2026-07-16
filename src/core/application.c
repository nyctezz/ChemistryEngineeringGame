#include "application.h"


void application_init(_application* app)
{
    // window errors:
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        abort();
    }

    if (!window_init(&app->window, "ChemistryEngineeringGame", 800, 600, false))
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

    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    uint32_t indices[] = {
        0, 1, 2
    };
    
    _mesh mesh;

    mesh_init(&mesh, vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(uint32_t));


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

        glClearColor(0.15f, 0.20f, 0.30f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        // use shader program
        //glUseProgram(shaderProgram); 
        //shader_use(&shader);
        renderer_run(&app->renderer);

        // render
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glBindVertexArray(0);
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