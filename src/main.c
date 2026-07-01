#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

#include "core/window.h"
#include "core/application.h"


/*
conventions I use:

*/


int main(int argc, char *argv[])
{
    _application app;

    application_init(&app);
    application_run(&app);

    application_destroy(&app);

    return 0;
}