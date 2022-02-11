#ifndef SPATIALQUERIES_H
#define SPATIALQUERIES_H

#include "../Structures/boostTypes.h"
#include "../Graph/graph.h"
#include "../GeoReach/RangeReach.h"


bool strictSpatialFirstQuery(Graph* SocialGraph, bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics);

bool strictSpatialFirstQueryWithMbr(LocationMap* spatialGraph, Graph* G, bgi::rtree<SccWithMbr, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics);

bool strictSpatialFirstQueryUsingBfl(Graph* SocialGraph, bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics);

bool strictSpatialFirstQueryUsingBflWithMbr(LocationMap* spatialGraph, Graph* G, bgi::rtree<SccWithMbr, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics);

bool spatialFirstQuery(Graph* SocialGraph, bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics);

bool spatialFirstQueryWithMbr(LocationMap* spatialGraph, Graph* G, bgi::rtree<SccWithMbr, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics);

bool spatialFirstQueryUsingBfl(Graph* SocialGraph, bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistic);

bool spatialFirstQueryUsingBflWithMbr(LocationMap* spatialGraph, Graph* G, bgi::rtree<SccWithMbr, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics);

#endif