#ifndef LOCATIONMAP_H
#define LOCATIONMAP_H

#include <unordered_map>
#include "location.h"
#include "../Structures/boostTypes.h"


class LocationMap {
public:

    unordered_map <int, Location> LocationScheme;
    unordered_map <int, pair<Location, Location>> LocationSchemeSCC;
    //key => (isMbr, [lowesrXpoass, lowerYpos, upperXpos, upperYpos, .....])
    unordered_map<int, spatialMbrRelation> Map;
    vector<coordinates> MinMaxCorners;

    LocationMap();

    //Augments the node with the given location.
    void addSpatial(int vertex, Location loc);
    void addSpatialToMap(int vertex, spatialMbrRelation);
    //Checks if the nodes has spatial data.
    bool existLocation(int node);
    void addMinMaxCorners(vector<coordinates> corners);
    vector<coordinates> getMinMaxCorners();
    // Returns the location of the node.
    spatialMbrRelation getLocation(int node);
    // Returns the complete Location Scheme.
    unordered_map <int, spatialMbrRelation> returnMap();
    void createMbrs(unordered_map<int, vector<int>>* SCCs);

    // Returns the location of the node.
    Location getVanillaLocation(int node);

    // Returns the complete Location Scheme.
    unordered_map <int, Location> returnVanillaMap();

    void printLocations();
    void printMap();
    void writeMapToFile(string fileName);
    void readFileForMap(string fileName);
};

#endif LOCATIONMAP_H;