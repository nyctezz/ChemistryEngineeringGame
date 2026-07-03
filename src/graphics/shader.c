#include "shader.h"


// helper functions:
static char* read_shader_file(const char* path) 
{
    FILE* file = fopen(path, "rb");

    if (!file) 
    {
        fprintf(stderr, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: %s\n", path);
        return NULL;
    }

    // find file size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memory (+1 for null terminator)
    char* buffer = malloc(length + 1);
    if (!buffer) 
    {
        fprintf(stderr, "ERROR::SHADER::MALLOC_FAILED for file: %s\n", path);
        fclose(file);
        return NULL;
    }

    // read and terminate string
    size_t read_bytes = fread(buffer, 1, length, file);
    buffer[read_bytes] = '\0';

    fclose(file);
    return buffer;
}

static int check_compile_errors(unsigned int shader, const char* type) 
{
    /* RETURNED ERROR CODES:
    0 - no errors found
    1 - errors found
    */

    int success;
    char info_log[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n---\n", type, info_log);
        return 1;
    }

    return 0;
}

static int check_linking_errors(unsigned int shader)
{
    /* RETURNED ERROR CODES:
    0 - no errors found
    1 - errors found
    */

    int success;
    char info_log[1024];

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shader, 1024, NULL, info_log);
        fprintf(stderr, "ERROR::PROGRAM_LINKING_ERROR: %s\n---\n", info_log);
        return 1;
    }

    return 0;
}
// ---


int shader_init(_shader* shader, const char* vert_shader_path, const char* frag_shader_path)
{
    /* RETURNED ERROR CODES:
    0 - success
    1 - failed to load both vertex and fragment shaders
    2 - failed to load vertex shader
    3 - failed to load fragment shader
    4 - compilation error of vertex shader
    5 - compilation error of fragment shader
    6 - shader linking error
    */


    char* vert_shader_code = read_shader_file(vert_shader_path);
    char* frag_shader_code = read_shader_file(frag_shader_path);

    if (!vert_shader_code && !frag_shader_code) 
    {
        free(vert_shader_code);
        free(frag_shader_code);
        return 1; 
    }
    else if (!vert_shader_code)
    {
        free(vert_shader_code);
        free(frag_shader_code);
        return 2; 
    }
    else if (!frag_shader_code)
    {
        free(vert_shader_code);
        free(frag_shader_code);
        return 3; 
    }



    // compile vertex shader:
    shader->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader->vertex_shader, 1, (const GLchar *const *)&vert_shader_code, NULL);
    glCompileShader(shader->vertex_shader);

    if (check_compile_errors(shader->vertex_shader, "VERTEX"))
    {
        glDeleteShader(shader->vertex_shader);
        free(vert_shader_code);
        free(frag_shader_code);
        return 4; 
    }
    

    // compile fragment shader:
    shader->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->fragment_shader, 1, (const GLchar *const *)&frag_shader_code, NULL);
    glCompileShader(shader->fragment_shader);

    if (check_compile_errors(shader->fragment_shader, "FRAGMENT"))
    {
        glDeleteShader(shader->vertex_shader);
        glDeleteShader(shader->fragment_shader);
        free(vert_shader_code);
        free(frag_shader_code);
        return 5; 
    }


    // link shaders into a program:
    shader->shader_program = glCreateProgram();
    glAttachShader(shader->shader_program, shader->vertex_shader);
    glAttachShader(shader->shader_program, shader->fragment_shader);
    glLinkProgram(shader->shader_program);

    if (check_linking_errors(shader->shader_program))
    {
        glDeleteShader(shader->vertex_shader);
        glDeleteShader(shader->fragment_shader);
        glDeleteProgram(shader->shader_program);
        free(vert_shader_code);
        free(frag_shader_code);
        return 6; 
    }


    // cleanupm, shaders are linked, we don't need them anymore
    glDeleteShader(shader->vertex_shader);
    glDeleteShader(shader->fragment_shader);

    free(vert_shader_code);
    free(frag_shader_code);


    return 0;
}

void shader_use(_shader* shader)
{
    glUseProgram(shader->shader_program);
}

void shader_destroy(_shader* shader)
{
    glDeleteProgram(shader->shader_program);
}