#include "CommandLineOptions.h"
#include "Data.hpp"
#include "Postprocessing.h"
#include "ModelType.h"

using namespace DNest5;
using namespace NRL2021;

int main(int argc, char** argv)
{
    Data::load("../nrl_data_2021.yaml");
    CommandLineOptions options(argc, argv);
    postprocess<ModelType>(options);
    return 0;
}

