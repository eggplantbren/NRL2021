#include "CommandLineOptions.h"
#include "Postprocessing.h"
#include "ModelType.h"

using namespace DNest5;
using namespace NRL2021;

int main(int argc, char** argv)
{
    CommandLineOptions options(argc, argv);
    postprocess<ModelType>(options);
    return 0;
}

