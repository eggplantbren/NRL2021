#ifndef DNest5_Template_MyModel_hpp
#define DNest5_Template_MyModel_hpp

#include <cmath>
#include <cstdlib>
#include "Data.hpp"
#include <ParameterNames.h>
#include <Tools/Misc.hpp>
#include <vector>

namespace NRL2021
{

using DNest5::ParameterNames, Tools::RNG;

class MyModel
{
    private:
        double sig;
        std::vector<double> ns;

    public:
        inline MyModel(RNG& rng);
        inline double perturb(RNG& rng);
        inline double log_likelihood() const;
        inline std::vector<char> to_blob() const;
        inline std::string to_string() const;
};

/* Implementations follow */

inline MyModel::MyModel(RNG& rng)
:ns(Data::get_num_teams())
{
    sig = 2.0*rng.rand();
    for(double& n: ns)
        n = rng.randn();
}

inline double MyModel::perturb(RNG& rng)
{
    return 0.0;
}

inline double MyModel::log_likelihood() const
{
    double logl = 0.0;

    std::vector<double> abilities(Data::get_num_teams());
    for(int i=0; i<Data::get_num_teams(); ++i)
        abilities[i] = sig*ns[i];

    const auto& matches = Data::get_matches();
    for(const auto& match: matches)
    {
        // probability of home team win
        double prob_htw = abilities[match.home_team]
                                /(abilities[match.home_team]
                                    + abilities[match.away_team]);
        if(match.home_team_won)
            logl += log(prob_htw);
        else
            logl += log(1.0 - prob_htw);
    }
    return logl;
}

inline std::vector<char> MyModel::to_blob() const
{
    std::vector<char> result((1 + ns.size())*sizeof(double));
    std::stringstream ss;
    ss.write((const char*)(&sig), sizeof(double));
    for(const double& n: ns)
        ss.write((const char*)(&n), sizeof(double));
    std::string s = ss.str();
    std::memcpy(&result, &s, result.size());
    return result;
}

inline std::string MyModel::to_string() const
{
    std::stringstream ss;
    ss << sig << ',';
    for(const double& n: ns)
        ss << n << ',';
    return ss.str();
}


inline void MyModel::from_blob(const std::vector<char>& bytes)
{
    std::memcpy(&sig, &bytes[0], sizeof(double));
    std::memcpy(&ns[0], &bytes[1], ns.size()*sizeof(double));
    return result;
}

} // namespace

#endif

