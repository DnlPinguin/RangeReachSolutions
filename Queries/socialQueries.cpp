#include "socialQueries.h"
#include "helper.h"

bool socialFirstQuery(Graph* socialGraph, LocationMap* spatialGraph, vector<queryParameter>::iterator queryParam,  socialFirstResult* statistics) 
{
    int node = queryParam->queryNode;
    box spatialRegion = queryParam->spatialRegion;
    spatialMbrRelation LocationNode;
    box mbr;
    vector<int> reachableNodes;
    
    #ifdef STATISTICS
        Timer clock;
        clock.start();
    #endif

    if (socialGraph->NodeBelongsToSCC.count(node) != 0){
        node = socialGraph->NodeBelongsToSCC[node];
    }

    vector<IntervalScheme>*  Intervals = &(socialGraph->IntervalSchemeGraphMap[node]);
    for (vector<IntervalScheme>::iterator iter = Intervals->begin() ; iter != Intervals->end(); iter++)
    {
        for (int it = iter->pre; it <= iter->post; it++) 
        {
            reachableNodes.push_back(socialGraph->nodeHasPostorder[it]);
        }
    }
    for (int i : reachableNodes)
    #ifdef STATISTICS 
        int amount_of_reachable_nodes = 0;
        double social_time = clock.stop();
        int spatial_range_test_counter = 0;
        clock.start();
    #endif

    for (int nodeToCheck : reachableNodes) {
        #ifdef STATISTICS
            amount_of_reachable_nodes++;
        #endif

        if (spatialGraph->existLocation(nodeToCheck))
        {
            LocationNode = spatialGraph->getLocation(nodeToCheck);

            mbr = box(point(LocationNode.spatialData[0], LocationNode.spatialData[1]), point(LocationNode.spatialData[0], LocationNode.spatialData[1]));
            if (!LocationNode.isMbr) 
            {
                #ifdef STATISTICS
                    spatial_range_test_counter++;
                #endif
                if (boost::geometry::intersects(spatialRegion, mbr))
                {
                    if (nodeToCheck != queryParam->queryNode) {
                        #ifdef STATISTICS
                            statistics->time_social = social_time;
                            statistics->time_spatial = clock.stop();
                            statistics->number_of_spatial_range_tests = spatial_range_test_counter;
                            statistics->reachable_nodes = amount_of_reachable_nodes;
                        #endif
                        return true;
                    }
                }
            }
            else
            {
                for (int i = 4; i != LocationNode.spatialData.size(); i++)
                {
                    #ifdef STATISTICS
                        spatial_range_test_counter++;
                    #endif
                    box mbr = box(point(LocationNode.spatialData[i], LocationNode.spatialData[(i + 1)]), point(LocationNode.spatialData[i], LocationNode.spatialData[(i + 1)]));
                    if (boost::geometry::intersects(spatialRegion, mbr))
                    {
                        if (nodeToCheck != queryParam->queryNode) {
                            #ifdef STATISTICS
                                statistics->time_social = social_time;
                                statistics->time_spatial = clock.stop();
                                statistics->number_of_spatial_range_tests = spatial_range_test_counter;
                                statistics->reachable_nodes = amount_of_reachable_nodes;
                            #endif
                            return true;
                        }
                    }
                }
            }
        }
    }
    #ifdef STATISTICS
        statistics->time_social = social_time;
        statistics->time_spatial = clock.stop();
        statistics->number_of_spatial_range_tests = spatial_range_test_counter;
        statistics->reachable_nodes = amount_of_reachable_nodes;
    #endif
    return false;
}

bool socialFirstQueryWithMbr(Graph* socialGraph, LocationMap* spatialGraph, vector<queryParameter>::iterator queryParam, socialFirstResult* statistics) {
    int node = queryParam->queryNode;
    box spatialRegion = queryParam->spatialRegion;
    if (socialGraph->NodeBelongsToSCC.count(node) != 0){
        node = socialGraph->NodeBelongsToSCC[node];
    }
    vector<int> reachableNodes;
    #ifdef STATISTICS
        Timer clock;
        clock.start();
    #endif

    vector<IntervalScheme>*  Intervals = &(socialGraph->IntervalSchemeGraphMap[node]);
    for (vector<IntervalScheme>::iterator iter = Intervals->begin() ; iter != Intervals->end(); iter++)
    {
        for (int it = iter->pre; it <= iter->post; it++) 
        {
            reachableNodes.push_back(socialGraph->nodeHasPostorder[it]);
        }
    }
    
    #ifdef STATISTICS
        int amount_of_reachable_nodes = 0;
        double social_time = clock.stop();
        int spatial_range_test_counter = 0;
        clock.start();
    #endif

    for (int nodeToCheck : reachableNodes) 
    {
        #ifdef STATISTICS
            amount_of_reachable_nodes++;
        #endif

        if (spatialGraph->existLocation(nodeToCheck))
        {
            spatialMbrRelation LocationNode = spatialGraph->getLocation(nodeToCheck);
            tuple<bool,int> res = checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion); 
            #ifdef STATISTICS
                spatial_range_test_counter = spatial_range_test_counter + get<1>(res);
            #endif
            if (get<0>(res))
            {
                if (queryParam->queryNode != nodeToCheck) {
                    #ifdef STATISTICS
                        statistics->time_social = social_time;
                        statistics->time_spatial = clock.stop();
                        statistics->number_of_spatial_range_tests = spatial_range_test_counter;
                        statistics->reachable_nodes = amount_of_reachable_nodes;
                    #endif
                    return true;
                }
            }
        }
    }

    #ifdef STATISTICS
        statistics->time_social = social_time;
        statistics->time_spatial = clock.stop();
        statistics->number_of_spatial_range_tests = spatial_range_test_counter;
        statistics->reachable_nodes = amount_of_reachable_nodes;
    #endif
    return false;
}

bool strictSocialFirstQuery(Graph* socialGraph, LocationMap* spatialGraph, vector<queryParameter>::iterator queryParam, socialFirstResult* statistics)
{
    int node = queryParam->queryNode;
    if (socialGraph->NodeBelongsToSCC.count(node) != 0){
        node = socialGraph->NodeBelongsToSCC[node];
    }
    box spatialRegion = queryParam->spatialRegion;
    #ifdef STATISTICS
        Timer clock;
        clock.start();
    #endif
    vector<IntervalScheme>*  Intervals = &(socialGraph->IntervalSchemeGraphMap[node]);
    for (vector<IntervalScheme>::iterator iter = Intervals->begin() ; iter != Intervals->end(); iter++)
    {
        for (int it = iter->pre; it <= iter->post; it++) 
        {
            int reachableNode = socialGraph->nodeHasPostorder[it];
            if (spatialGraph->existLocation(reachableNode))
            {
                spatialMbrRelation LocationNode = spatialGraph->getLocation(reachableNode);
                box mbr = box(point(LocationNode.spatialData[0], LocationNode.spatialData[1]), point(LocationNode.spatialData[0], LocationNode.spatialData[1]));
                if (!LocationNode.isMbr) 
                {
                    // spatial_range_test_counter++;
                    if (boost::geometry::intersects(spatialRegion, mbr))
                    {
                        if (reachableNode != queryParam->queryNode) {

                            // statistics->time_social = social_time;
                            // statistics->time_spatial = clock.stop();
                            // statistics->number_of_spatial_range_tests = spatial_range_test_counter;
                            // statistics->reachable_nodes = amount_of_reachable_nodes;
                            return true;
                        }
                    }
                }
                else
                {
                    for (int i = 4; i != LocationNode.spatialData.size(); i++)
                    {
                        // spatial_range_test_counter++;
                        box mbr = box(point(LocationNode.spatialData[i], LocationNode.spatialData[(i + 1)]), point(LocationNode.spatialData[i], LocationNode.spatialData[(i + 1)]));
                        if (boost::geometry::intersects(spatialRegion, mbr))
                        {
                            if (reachableNode != queryParam->queryNode) {
                                // statistics->time_social = social_time;
                                // statistics->time_spatial = clock.stop();
                                // statistics->number_of_spatial_range_tests = spatial_range_test_counter;
                                // statistics->reachable_nodes = amount_of_reachable_nodes;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool strictSocialFirstQueryWithMbr(Graph* socialGraph, LocationMap* spatialGraph, vector<queryParameter>::iterator queryParam, socialFirstResult* statistics)
{
    int node = queryParam->queryNode;
    if (socialGraph->NodeBelongsToSCC.count(node) != 0){
        node = socialGraph->NodeBelongsToSCC[node];
    }
    box spatialRegion = queryParam->spatialRegion;

    vector<IntervalScheme>*  Intervals = &(socialGraph->IntervalSchemeGraphMap[node]);
    for (vector<IntervalScheme>::iterator iter = Intervals->begin() ; iter != Intervals->end(); iter++)
    {
        for (int it = iter->pre; it <= iter->post; it++) 
        {
            int reachableNode = socialGraph->nodeHasPostorder[it];
            if (spatialGraph->existLocation(reachableNode))
            {

                spatialMbrRelation LocationNode = spatialGraph->getLocation(reachableNode);
                tuple<bool,int> res = checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion); 
            
                if (get<0>(res))
                {
                    if (queryParam->queryNode != reachableNode)
                    {

                        // statistics->time_social = social_time;
                        // statistics->time_spatial = clock.stop();
                        // statistics->number_of_spatial_range_tests = spatial_range_test_counter;
                        // statistics->reachable_nodes = amount_of_reachable_nodes;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

