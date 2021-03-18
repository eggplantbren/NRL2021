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
        std::vector<double> abilities;
        inline void compute_abilities();

    public:
        inline MyModel(RNG& rng);
        inline double perturb(RNG& rng);
        inline double log_likelihood() const;
        inline std::vector<char> to_blob() const;
        inline void from_blob(const std::vector<char>& bytes);
        inline std::string to_string() const;
        static ParameterNames parameter_names;
};

/* Implementations follow */

ParameterNames MyModel::parameter_names;

inline MyModel::MyModel(RNG& rng)
:ns(Data::get_num_teams())
,abilities(Data::get_num_teams())
{
    sig = 2.0*rng.rand();
    for(double& n: ns)
        n = rng.randn();
    compute_abilities();

    if(parameter_names.size() == 0)
    {
        parameter_names.add("sig");
        for(int i=0; i<Data::get_num_teams(); ++i)
        {
            std::stringstream ss;
            ss << "abilities[" << i << "]";
            parameter_names.add(ss.str());
        }
    }
}

inline void MyModel::compute_abilities()
{
    for(int i=0; i<Data::get_num_teams(); ++i)
        abilities[i] = exp(sig*ns[i]);
}

inline double MyModel::perturb(RNG& rng)
{
    double logh = 0.0;

    if(rng.rand() <= 0.25)
    {
        sig += 2.0*rng.randh();
        Tools::wrap(sig, 0.0, 2.0);
    }
    else
    {
        int k = rng.rand_int(ns.size());
        logh -= -0.5*ns[k]*ns[k];
        ns[k] += rng.randh();
        logh += -0.5*ns[k]*ns[k];
    }
    compute_abilities();

    return logh;
}

inline double MyModel::log_likelihood() const
{
    double logl = 0.0;

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
    std::memcpy(&result[0], &s[0], result.size());
    return result;
}

inline std::string MyModel::to_string() const
{
    std::stringstream ss;
    ss << sig << ',';
    for(int i=0; i<int(abilities.size()); ++i)
    {
        ss << abilities[i];
        if(i != int(abilities.size()) - 1)
            ss << ',';
    }
    return ss.str();
}


inline void MyModel::from_blob(const std::vector<char>& bytes)
{
    std::memcpy(&sig, &bytes[0], sizeof(double));
    std::memcpy(&ns[0], &bytes[0]+1, ns.size()*sizeof(double));
    compute_abilities();
}

} // namespace

#endif

