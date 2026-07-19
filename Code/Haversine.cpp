#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include <string>
#include <fstream>
#include "include/rapidjson/document.h"
#include "include/rapidjson/istreamwrapper.h"

const int earthRadius = 6371;
using namespace std;

struct Location
{
    double latitude;
    double longitude;
    string name;
};

inline double toRadians(const double &degree)
{
    return degree * M_PI / 180.0;
}

double haversineDistance(const Location &loc1, const Location &loc2)
{
    double φ1 = toRadians(loc1.latitude);
    double λ1 = toRadians(loc1.longitude);
    double φ2 = toRadians(loc2.latitude);
    double λ2 = toRadians(loc2.longitude);

    double Δφ = φ2 - φ1;
    double Δλ = λ2 - λ1;

    double term1 = sin(Δφ / 2) * sin(Δφ / 2);
    double term2 = cos(φ1) * sin(Δλ / 2) * sin(Δλ / 2);
    double distance = 2 * earthRadius * asin(sqrt(term1 + term2));

    return distance;
}

Location haversineMidpoint(const Location &loc1, const Location &loc2)
{
    double φ1 = toRadians(loc1.latitude);
    double λ1 = toRadians(loc1.longitude);
    double φ2 = toRadians(loc2.latitude);
    double λ2 = toRadians(loc2.longitude);

    double Bx = cos(φ2) * cos(λ2 - λ1);
    double By = cos(φ2) * sin(λ2 - λ1);

    double φm = atan2(sin(φ1) + sin(φ2), sqrt((cos(φ1) + Bx) * (cos(φ1) + Bx) + By * By));
    double λm = λ1 + atan2(By, cos(φ1) + Bx);

    Location midpoint;
    midpoint.latitude = φm * 180.0 / M_PI;
    midpoint.longitude = λm * 180.0 / M_PI;
    midpoint.name = "Midpoint";

    return midpoint;
}

Location findNearestLocation(const Location &midpoint, const vector<Location> &locations)
{
    Location nearestLocation;
    double minDistance = numeric_limits<double>::max();

    for (const auto &loc : locations)
    {
        double distance = haversineDistance(midpoint, loc);
        if (distance < minDistance)
        {
            minDistance = distance;
            nearestLocation = loc;
        }
    }

    return nearestLocation;
}

vector<Location> readLocationsFromJSON(const string filename)
{
    vector<Location> locations;
    ifstream ifs(filename);

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    for (const auto &item : doc.GetArray())
    {
        Location loc;
        loc.latitude = item["latitude"].GetDouble();
        loc.longitude = item["longitude"].GetDouble();
        loc.name = item["name"].GetString();
        locations.push_back(loc);
    }

    return locations;
}

int main(void)
{
    Location loc1 = {35.6762, 139.6503, "Tokyo"};
    Location loc2 = {-22.9068, -43.1729, "Rio de Janeiro"};

    Location midpoint = haversineMidpoint(loc1, loc2);
    cout << "Midpoint: (" << midpoint.latitude << ", " << midpoint.longitude << ")\n";

    vector<Location> locations = readLocationsFromJSON("locations.json");

    Location nearestLocation = findNearestLocation(midpoint, locations);
    cout << "Nearest Location: " << nearestLocation.name << " (" << nearestLocation.latitude << ", " << nearestLocation.longitude << ")\n";
}