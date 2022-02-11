#ifndef QUERYHELPER_H
#define QUERYHELPER_H

#include "../Structures/boostTypes.h"
#include "../Graph/graph.h"

bool isContained(box rectangleOne, box rectangleTwo);

bool checkIfOneEdgeIsContainedInRectangle(box rectangle, box spatialRegion);

bool checkIfRectangleIsContainedInRegion(box rectangle, box spatialRegion);

tuple<bool,int> checkIfNodeIsInSpatialRegion(bool isMbr, vector<coordinates> LocationNode, box spatialRegion);

void getNodesInsideSpatialField(bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, box spatialRegion, vector<int>* nodes, double* time);

int computeCardinality( bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, box queryParam);

#endif