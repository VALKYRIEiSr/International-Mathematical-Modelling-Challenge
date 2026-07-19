#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Team
{
    string name;
    vector<int> scheduledWeeks;
    Team(const string &n) : name(n) {}
};

struct Matchup
{
    Team *team1;
    Team *team2;
    int week = 0;

    Matchup(Team *t1, Team *t2) : team1(t1), team2(t2) {}

    int getSaturation() const
    {
        if (week != 0)
            return INT_MAX;
        return team1->scheduledWeeks.size() + team2->scheduledWeeks.size();
    }

    bool operator<(const Matchup &other) const
    {
        return this->getSaturation() < other.getSaturation();
    }
};

vector<Matchup> createMatchList(vector<vector<Team>> &allTeams)
{
    vector<Matchup> matchList;

    for (auto &tier : allTeams)
    {
        matchList.push_back(Matchup(&tier[0], &tier[1]));
        matchList.push_back(Matchup(&tier[0], &tier[2]));
        matchList.push_back(Matchup(&tier[1], &tier[3]));
        matchList.push_back(Matchup(&tier[2], &tier[3]));
    }

    for (size_t i = 0; i < allTeams.size(); i++)
    {
        for (size_t j = i + 1; j < allTeams.size(); j++)
        {
            vector<Team> &tier1 = allTeams[i];
            vector<Team> &tier2 = allTeams[j];

            matchList.push_back(Matchup(&tier1[0], &tier2[0]));
            matchList.push_back(Matchup(&tier1[0], &tier2[1]));
            matchList.push_back(Matchup(&tier1[1], &tier2[1]));
            matchList.push_back(Matchup(&tier1[1], &tier2[2]));
            matchList.push_back(Matchup(&tier1[2], &tier2[2]));
            matchList.push_back(Matchup(&tier1[2], &tier2[3]));
            matchList.push_back(Matchup(&tier1[3], &tier2[3]));
            matchList.push_back(Matchup(&tier1[3], &tier2[0]));
        }
    }

    return matchList;
}

bool isTeamScheduledInWeek(Team *team, int week)
{
    for (int w : team->scheduledWeeks)
    {
        if (w == week)
            return true;
    }
    return false;
}

void scheduleMatches(vector<Matchup> &matches)
{
    while (true)
    {
        auto minSat = min_element(matches.begin(), matches.end());

        if (minSat->getSaturation() == INT_MAX)
            break;

        int week = 1;
        while (isTeamScheduledInWeek(minSat->team1, week) ||
               isTeamScheduledInWeek(minSat->team2, week))
        {
            week++;
        }

        minSat->week = week;
        minSat->team1->scheduledWeeks.push_back(week);
        minSat->team2->scheduledWeeks.push_back(week);
    }
}

void printMatchList(const vector<Matchup> &matchList)
{
    int maxWeek = 0;
    for (const auto &match : matchList)
    {
        maxWeek = max(maxWeek, match.week);
    }

    for (int week = 1; week <= maxWeek; week++)
    {
        cout << "\nWeek " << week << " Matches:" << endl;
        for (const auto &match : matchList)
        {
            if (match.week == week)
            {
                cout << match.team1->name << " vs " << match.team2->name << endl;
            }
        }
    }
}

int main()
{
    vector<vector<Team>> allTeams = {
        {Team("Paris Saint-Germain FC"), Team("Real Madrid CF"), Team("FC Barcelona "), Team("Palmeiras")},
        {Team("Al Hilal"), Team("Corinthians"), Team("Internacional"), Team("Al Ahly")},
        {Team("Club América"), Team("Al Nassr"), Team("Mamelodi Sundowns FC"), Team("Esperance de Tunis")},
        {Team("Cruz Azul"), Team("Toluca FC"), Team("Pyramids FC"), Team("Persepolis ")},
        {Team("Kawasaki Frontale"), Team("Al Saad"), Team("Auckland City"), Team("Al Ahli")},
        {Team("Sepahan"), Team("Al Ittihad"), Team("Yokohama F. Marinos"), Team("Team Wellington")}};


    auto matches = createMatchList(allTeams);
    scheduleMatches(matches);
    printMatchList(matches);

    return 0;
}
