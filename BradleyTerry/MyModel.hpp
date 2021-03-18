#ifndef DNest5_Template_MyModel_hpp
#define DNest5_Template_MyModel_hpp

#include <cmath>
#include "Data.hpp"
#include <UniformModel.hpp>
#include <Tools/Misc.hpp>

namespace NRL2021
{

using DNest5::ParameterNames, Tools::RNG;

// The "Curiously Recursive" template argument is so that the correct
// naming scheme is used.
class MyModel : public DNest5::UniformModel<16, MyModel>
{
    private:

    public:
        inline MyModel(RNG& rng);
        inline void us_to_params();
        inline double log_likelihood() const;
};

/* Implementations follow */

inline MyModel::MyModel(RNG& rng)
:UniformModel(rng)
{
    us_to_params();
}

inline void MyModel::us_to_params()
{
    xs = std::vector<double>(us.size());
    for(size_t i=0; i<xs.size(); ++i)
        xs[i] = exp(Tools::qnorm(us[i]));
}

inline double MyModel::log_likelihood() const
{
    double logl = 0.0;

    const auto& matches = Data::get_matches();
    for(const auto& match: matches)
    {
        // probability of home team win
        double prob_htw = xs[match.home_team]
                                /(xs[match.home_team] + xs[match.away_team]);
        if(match.home_team_won)
            logl += log(prob_htw);
        else
            logl += log(1.0 - prob_htw);
    }
    return logl;
}

} // namespace

#endif

