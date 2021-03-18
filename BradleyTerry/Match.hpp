#ifndef NRL2021_Match_hpp
#define NRL2021_Match_hpp

#include <ostream>

namespace NRL2021
{

struct Match
{
    int home_team;
    int away_team;
    bool home_team_won;
};

inline std::ostream& operator << (std::ostream& out, const Match& m);

/* IMPLEMENTATIONS FOLLOW */

inline std::ostream& operator << (std::ostream& out, const Match& m)
{
    out << m.home_team << ' ' << m.away_team << ' ' << m.home_team_won;
    return out;
}

} // namespace

#endif

