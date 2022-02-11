#ifndef RTREE_H
#define RTREE_H

#include "../GeoReach/RangeReach.h"

rTreeLines* buildThreeDimensionalRtree(Graph* HybridGraph, LocationMap* LocationGraph);

rTreeCubes* buildThreeDimensionalRtreeWithCuboids(Graph* HybridGraph, LocationMap* LocationGraph);

rTreePoints* buildThreeDimensionalRtreeWithPoints(Graph* HybridGraph, LocationMap* LocationGraph);

rTreeCubes* buildThreeDimesionalRTreeWithPlanes(Graph* HybridGraph, LocationMap* LocationGraph);

rTreePlanes* buildTwoDimensionalRtree(LocationMap* LocationGraph);

rTreeSccPlanes* buildTwoDimensionalRtreeWithMbr(LocationMap* spatialGraph);

#endif