#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cmath>
#include "include/rapidjson/document.h"
#include "include/rapidjson/istreamwrapper.h"

using namespace std;

struct Match
{
    string date;
    string team1;
    string team2;
    int goalDifference = 0;

    bool operator<(const Match &other) const { return date < other.date; }
    bool operator>(const Match &other) const { return date > other.date; }
};

struct TeamElo
{
    string name;
    int elo = 1500;

    bool operator<(const TeamElo &other) const { return elo < other.elo; }
    bool operator>(const TeamElo &other) const { return elo > other.elo; }
};

double Outcome(const int ELO_A, const int ELO_B, const int SD, const double GD)
{
    double EGD = (ELO_A - ELO_B) / 200;
    double a = (M_PI / (sqrt(6) * SD)) * (ELO_A - ELO_B);
    double b = (M_PI / (sqrt(6) * SD)) * 200;

    double pos_a_func = 1 + exp(a - (b * GD));
    double neg_a_func = 1 + exp(-a + (b * GD));

    if (GD > EGD)
        return (2 / b) * log( 2 / pos_a_func);
    if (GD < EGD)
        return (2 / b) * log(neg_a_func / 2 );
    return 0;
}


vector<Match> parseJsonFile(const string &filepath)
{
    ifstream ifs(filepath);
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    vector<Match> matches;
    for (const auto &matchItem : doc["matches"].GetArray())
    {
        Match m;
        m.date = matchItem["date"].GetString();
        m.team1 = matchItem["team1"].GetString();
        m.team2 = matchItem["team2"].GetString();

        if (matchItem.HasMember("score") && matchItem["score"].HasMember("ft"))
        {
            const auto &ft = matchItem["score"]["ft"];
            m.goalDifference = ft[0].GetInt() - ft[1].GetInt();
        }
        matches.push_back(m);
    }
    return matches;
}

vector<Match> processDataDirectory()
{
    vector<Match> allMatches;
    for (const auto &entry : filesystem::directory_iterator("data"))
    {
        
        auto fileMatches = parseJsonFile(entry.path().string());
        allMatches.insert(allMatches.end(), fileMatches.begin(), fileMatches.end());

    }
    sort(allMatches.begin(), allMatches.end());
    return allMatches;
}

vector<TeamElo> getAllTeamsWithElo(const vector<Match>& matches) {
    vector<TeamElo> teamElos;
    
    for (const auto& match : matches) {
        bool found1 = false;
        bool found2 = false;
        
        for (const auto& team : teamElos) {
            if (team.name == match.team1) found1 = true;
            if (team.name == match.team2) found2 = true;
        }
        
        if (!found1) teamElos.push_back({match.team1, 1500});
        if (!found2) teamElos.push_back({match.team2, 1500});
    }
    
    return teamElos;
}

void updateElos(vector<TeamElo>& teamElos, const vector<Match>& matches) {
    const int K = 100;
    const int SD = 250;

    for (const auto& m : matches) {

        int* elo1 = nullptr;
        int* elo2 = nullptr;
        
        for (auto& team : teamElos) {
            if (team.name == m.team1) elo1 = &team.elo;
            if (team.name == m.team2) elo2 = &team.elo;
            if (elo1 && elo2) break;
        }

        double change1 = Outcome(*elo1, *elo2, SD, m.goalDifference);
        double change2 = Outcome(*elo2, *elo1, SD, -m.goalDifference);
        
        *elo1 += K * change1;
        *elo2 += K * change2;
    }
}

int main()
{
    auto matches = processDataDirectory();
    cout << "Total matches: " << matches.size() << "\n";

    auto teamElos = getAllTeamsWithElo(matches); 
    updateElos(teamElos, matches);

    sort(teamElos.begin(), teamElos.end());

    for (const auto &te : teamElos)
        cout << te.name << ": " << te.elo << "\n";

    return 0;
}