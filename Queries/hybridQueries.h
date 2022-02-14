
#ifndef HYBRIDQUERIES_H
#define HYBRIDQUERIES_H

#include "../Structures/boostTypes.h"
#include "../Graph/graph.h"

bool hybridQuery(int nodeDimension, rTreeLines* rTree, vector<queryParameter>::iterator queryParam, int* counter);

bool hybridQueryWithCubes(rTreeCubes rTree, box spatialRegion, LocationMap* spatialGraph, int nodeDimension, int* counter);

bool hybridQueryReverse(vector<IntervalScheme>* interval, rTreePoints rTree, vector<queryParameter>::iterator queryParam, int* counter);

bool hybridQueryWithCubesReverse(vector<IntervalScheme>* interval, rTreeCubes rTree, box spatialRegion, LocationMap* spatialGraph, int* counter);
#endif