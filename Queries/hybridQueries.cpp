#include "hybridQueries.h"
#include "helper.h"

bool hybridQuery(int nodeDimension, rTreeLines* rTree, vector<queryParameter>::iterator queryParam, int* counter)
{
    box spatialRegion = queryParam->spatialRegion;

    plane planeForQuerying(
        threeDimPoint(spatialRegion.min_corner().get<0>(), spatialRegion.min_corner().get<1>(), nodeDimension),
        threeDimPoint(spatialRegion.max_corner().get<0>(), spatialRegion.max_corner().get<1>(), nodeDimension)
    );
    for (auto it = rTree->qbegin(bgi::intersects(planeForQuerying)); it != rTree->qend(); ++it)
    {  
        if (queryParam->queryNode != it->second){
            return true;
        }
    }
    return false;
}

bool hybridQueryWithCubes(bgi::rtree<cuboidStructure, bgi::linear<16>>* rTree, box spatialRegion, LocationMap* spatialGraph, int nodeDimension,int queryNode,  int* counter) 
{
    plane planeForQuerying(
        threeDimPoint(spatialRegion.min_corner().get<0>(), spatialRegion.min_corner().get<1>(), nodeDimension),
        threeDimPoint(spatialRegion.max_corner().get<0>(), spatialRegion.max_corner().get<1>(), nodeDimension)
    );

    for (auto it = rTree->qbegin(bgi::intersects(planeForQuerying)); it != rTree->qend(); ++it)
    {
        counter++;
        spatialMbrRelation LocationNode = spatialGraph->getLocation(it->second);
        tuple<bool,int> res = checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion);

        if (get<0>(res))
        {
            if (queryNode != it->second) 
                return true;
        }
    }
    return false;
}

bool hybridQueryReverse(vector<IntervalScheme>* intervals, rTreePoints* rTree, vector<queryParameter>::iterator queryParam, int* counter){

    box spatialRegion = queryParam->spatialRegion;

    vector<IntervalScheme>::iterator interval;  


    for (interval = intervals->begin(); interval != intervals->end(); interval++)
    {
        plane planeForQuerying(
            threeDimPoint(spatialRegion.min_corner().get<0>(), spatialRegion.min_corner().get<1>(), interval->pre),
            threeDimPoint(spatialRegion.max_corner().get<0>(), spatialRegion.max_corner().get<1>(), interval->post)
        );


        for (auto it = rTree->qbegin(bgi::intersects(planeForQuerying)); it != rTree->qend(); ++it)
        {


            if (queryParam->queryNode != it->second){
                return true;
            }

        }
    }
    
    return false;
}

bool hybridQueryWithCubesReverse(vector<IntervalScheme>* intervals, rTreeCubes* rTree, box spatialRegion, LocationMap* spatialGraph, int queryNode, int* counter){
    
    vector<IntervalScheme>::iterator interval;  
    for (interval = intervals->begin(); interval != intervals->end(); interval++)
    {
        plane planeForQuerying(
            threeDimPoint(spatialRegion.min_corner().get<0>(), spatialRegion.min_corner().get<1>(), interval->pre),
            threeDimPoint(spatialRegion.max_corner().get<0>(), spatialRegion.max_corner().get<1>(), interval->post)
        );

        for (auto it = rTree->qbegin(bgi::intersects(planeForQuerying)); it != rTree->qend(); ++it)
        {
            counter++;
            spatialMbrRelation LocationNode = spatialGraph->getLocation(it->second);
            tuple<bool,int> res = checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion);
            if (get<0>(res))
            {
                if (queryNode != it->second) 
                    return true;
            }
        }
    }
    return false;
}

