//#include <stdio.h>
//#include <stdbool.h>

//#include <SDL3/SDL.h>
//#include <GLAD/glad.h>

//#include "core/window.h"
#include "core/application.h"
//#include "core/renderer.h"


/*
conventions I use:

if function returns many errors, 0 is success, {1, 2, ...} are error codes
*/


int main(int argc, char *argv[])
{
    _application app;

    application_init(&app);
    application_run(&app);

    application_destroy(&app);

    return 0;
}