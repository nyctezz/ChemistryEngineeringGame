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


    // Define the triangle points (x, y, z)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind and upload data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set attribute pointers (tells OpenGL how to read the buffer)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind to prevent accidental changes
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



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
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        //---

        SDL_GL_SwapWindow(app->window.handle);
    }
}
    
void application_destroy(_application* app)
{
    window_destroy(&app->window);
    renderer_destroy(&app->renderer);
    SDL_Quit();
}