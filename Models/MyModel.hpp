#ifndef DNest5_Template_MyModel_hpp
#define DNest5_Template_MyModel_hpp

#include <cmath>
#include <cstdlib>
#include "Data.hpp"
#include <iomanip>
#include <ParameterNames.h>
#include <Tools/Misc.hpp>
#include <vector>

namespace NRL2021
{

using DNest5::ParameterNames, Tools::RNG;

class MyModel
{
    private:
        double sig_log_abilities;
        double home_advantage;
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
    sig_log_abilities = 2.0*rng.rand();
    home_advantage = exp(rng.randn());
    for(double& n: ns)
        n = rng.randn();
    compute_abilities();

    if(parameter_names.size() == 0)
    {
        parameter_names.add("sig_log_abilities");
        parameter_names.add("home_advantage");
        for(int i=0; i<Data::get_num_teams(); ++i)
        {
            std::stringstream ss;
            ss << "abilities[" << Data::get_team_name(i) << "]";
            parameter_names.add(ss.str());
        }
    }
}

inline void MyModel::compute_abilities()
{
    double tot = 0.0;
    for(int i=0; i<Data::get_num_teams(); ++i)
    {
        abilities[i] = sig_log_abilities*ns[i];
        tot += abilities[i];
    }
    double mean = tot/abilities.size();

    for(double& a: abilities)
        a = exp(a - mean);
}

inline double MyModel::perturb(RNG& rng)
{
    double logh = 0.0;

    if(rng.rand() <= 0.5)
    {
        int which = rng.rand_int(2);
        if(which == 0)
        {
            sig_log_abilities += 2.0*rng.randh();
            Tools::wrap(sig_log_abilities, 0.0, 2.0);
        }
        else
        {
            home_advantage = log(home_advantage);
            logh -= -0.5*pow(home_advantage, 2);
            home_advantage += rng.randh();
            logh += -0.5*pow(home_advantage, 2);
            home_advantage =exp(home_advantage);
        }
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
        double prob_htw = abilities[match.home_team]*home_advantage
                                /(abilities[match.home_team]*home_advantage
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
    std::vector<char> result((2 + ns.size())*sizeof(double));
    std::stringstream ss;
    ss.write((const char*)(&sig_log_abilities), sizeof(double));
    ss.write((const char*)(&home_advantage), sizeof(double));
    for(const double& n: ns)
        ss.write((const char*)(&n), sizeof(double));
    std::string s = ss.str();
    std::memcpy(&result[0], &s[0], result.size());
    return result;
}

inline std::string MyModel::to_string() const
{
    std::stringstream ss;
    ss << std::setprecision(10);
    ss << sig_log_abilities << ',' << home_advantage << ',';
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
    std::memcpy(&sig_log_abilities, &bytes[0], sizeof(double));
    std::memcpy(&home_advantage, &bytes[0] + sizeof(double), sizeof(double));
    std::memcpy(&ns[0], &bytes[0]+2*sizeof(double), ns.size()*sizeof(double));
    compute_abilities();
}

} // namespace

#endif

