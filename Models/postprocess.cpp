#include "CommandLineOptions.h"
#include "Data.hpp"
#include "Postprocessing.h"
#include "ModelType.h"

using namespace DNest5;
using namespace NRL2021;

int main(int argc, char** argv)
{
    // This is all just to get the columns right
    Data::load("../nrl_data_2021.yaml");
    RNG rng;
    MyModel m(rng);

    CommandLineOptions options(argc, argv);
    postprocess<ModelType>(options);
    return 0;
}

