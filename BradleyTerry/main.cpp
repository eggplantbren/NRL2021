#include <iostream>
#include <Misc.h>
#include <Sampler.hpp>
#include "Data.hpp"
#include "ModelType.h"

using DNest5::Options, DNest5::Sampler;
using namespace NRL2021;

int main()
{
    Data::load("../nrl_data_2021.yaml");
//    NRL2021::Data::print(std::cout);
    DNest5::clear_output_dir();
    Sampler<ModelType> sampler(Options("options.yaml"));
    sampler.run();
    return 0;
}

