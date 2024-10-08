#include <iostream>
#include <string>
#include "core/Input.h"
#include "core/Ntolen.h"
#include "core/widget/Window.h"

using namespace ntolen;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Invalid argument: need a file" << std::endl;
    }

    Ntolen *app = Ntolen::instance();
    Input input;
    app->setInputProcessor(&input);
    MSCInterpretResult result = app->runtime()->runFile(argv[1]);
    auto code = app->runtime()->exitCode();
    Ntolen::clean();
    if (code != 0)
    {
        return code;
    }

    // Exit with an error code if the script failed.
    if (result == RESULT_RUNTIME_ERROR)
        return 70; // EX_SOFTWARE.
    // TODO: 65 is impossible now and will need to be handled inside `cli.msc`
    if (result == RESULT_COMPILATION_ERROR)
        return 65; // EX_DATAERR.

    return 0;
}

/***
 * main ==> load main file ==>
 *
 *
 *
 *
 *
 *
 * */