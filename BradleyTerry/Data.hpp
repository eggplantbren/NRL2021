#ifndef NRL2021_Data_hpp
#define NRL2021_Data_hpp

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "Match.hpp"

namespace NRL2021
{

class Data
{
    private:

        // Map from team name to integer ID of each team.
        static std::map<std::string, int> teams;
        static std::vector<Match> matches;

    public:
        static inline void load(const char* filename);
        static inline void print(std::ostream& out);

};

/* IMPLEMENTATIONS FOLLOW */

// Empty map and vector
std::map<std::string, int> Data::teams;
std::vector<Match> Data::matches;

inline void Data::load(const char* filename)
{
    // Clear existing data
    teams.clear();
    matches.clear();

    YAML::Node root = YAML::LoadFile(filename);

    int i = 0;
    for(auto it=root.begin(); it != root.end(); ++it)
    {
        for(const auto& match: (*it)["matches"])
        {
            // Add teams if necessary
            std::string home_team = match["home_team"].as<std::string>();
            std::string away_team = match["away_team"].as<std::string>();
            for(const auto& team: {home_team, away_team})
            {
                if(!teams.contains(team))
                    teams.emplace(team, i++);
            }

            // Match results
            std::vector<int> scores = match["result"].as<std::vector<int>>();
            Match m{teams[home_team], teams[away_team], scores[0] > scores[1]};
            matches.emplace_back(std::move(m));
        }
    }
}

inline void Data::print(std::ostream& out)
{
    for(const auto& match: matches)
        out << match << std::endl;
}

} // namespace

#endif

