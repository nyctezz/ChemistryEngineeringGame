#include "core/application.h"


int main(int argc, char *argv[])
{
    _application app;

    application_init(&app);
    application_run(&app);
    application_destroy(&app);

    return 0;
}

/*
=-=-= conventions I use: =-=-=

[1] functions can return different kinds of errors, 0 is success, {1, 2, ...} are error codes

[2] I use '_' before struct names e.g. _structname

[3] in .h modules I use '#pragma once' instead of include guards

*/