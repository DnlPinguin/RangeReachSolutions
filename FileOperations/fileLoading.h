#ifndef FILELOADING_H
#define FILELOADING_H

#include "../Structures/boostTypes.h"
#include "../Graph/graph.h"
#include <string>
#include "../Spatial/LocationMap.h"
#include <fstream>
#include <iostream>


void loadFile(string fileName, Graph* graphToBuild);

void loadGeoFileData(string fileName, LocationMap* locs);

void loadSuperConnectedComponents(string fileName, Graph* graphToExpand, LocationMap* LocationGraph);

void loadVanillaGeoFileData(string fileName, LocationMap* locs);

vector<queryParameter>* readQueries(string fileName);

vector<SpatialNode>* readGeoFile(string fileName, LocationMap* spatialGraph);

#endif