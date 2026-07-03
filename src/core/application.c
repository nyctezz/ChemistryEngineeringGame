#include "application.h"


void application_init(_application* app)
{
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
    }
}

void application_run(_application* app)
{
    app->is_running = true;



    //---
    const char* vertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() { gl_Position = vec4(aPos, 1.0); }";

    const char* fragmentShaderSource = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() { FragColor = vec4(1.0, 0.5, 0.2, 1.0); }"; // Orange color



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





    // 1. Compile Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 2. Compile Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 3. Link Shaders into a Program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 4. Cleanup (shaders are linked, we don't need them anymore)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    //---



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

        //---

        // Inside your while loop
        glClearColor(0.15f, 0.20f, 0.30f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        // Activate the program
        glUseProgram(shaderProgram); 

        // Render
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
    SDL_Quit();
}