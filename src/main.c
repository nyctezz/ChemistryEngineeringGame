#include "core/application.h"



/*
conventions I use:

-> if function can return different kinds of errors, 0 is success, {1, 2, ...} are error codes

-> I use '_' before struct names e.g. _structname
*/



int main(int argc, char *argv[])
{
    _application app;

    application_init(&app);
    application_run(&app);

    application_destroy(&app);

    return 0;
}