#ifndef SOCIALQUERIES_H
#define SOCIALQUERIES_H

#include "../Structures/boostTypes.h"
#include "../Graph/graph.h"
#include "../GeoReach/RangeReach.h"

bool  socialFirstQuery(Graph* socialGraph, LocationMap* spatialGraph, vector<queryParameter>::iterator queryParam, socialFirstResult* statistics);

bool  socialFirstQueryWithMbr(Graph* socialGraph, LocationMap* spatialGraph, vector<queryParameter>::iterator queryParam, socialFirstResult* statistics);

bool  strictSocialFirstQuery(Graph* socialGraph, LocationMap* spatialGraph, vector<queryParameter>::iterator queryParam, socialFirstResult* statistics);

bool  strictSocialFirstQueryWithMbr(Graph* socialGraph, LocationMap* spatialGraph, vector<queryParameter>::iterator queryParam, socialFirstResult* statistics);

#endif