#include "spatialQueries.h"
#include "helper.h"

bool spatialFirstQuery(Graph* SocialGraph, bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics)
{
    int node = queryParam->queryNode;
    box spatialRegion = queryParam->spatialRegion;
    box mbr =     queryParam->spatialRegion;

    cout << "im here" << endl;
    for (auto it = rTree->qbegin(bgi::intersects(spatialRegion)); it != rTree->qend(); ++it)
    {
        cout << node << " => " << it->second << endl;;
        if (SocialGraph->reachNode(node, it->second)) 
        {
            return true;
        }
    }
    return false;
}

bool spatialFirstQueryWithMbr(LocationMap* spatialGraph, Graph* socialGraph, bgi::rtree<SccWithMbr, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics)
{
    // cout << "run Query: " << queryParam->queryNode << endl;;
    int node = queryParam->queryNode;
    box spatialRegion = queryParam->spatialRegion;

    for (auto it = rTree->qbegin(bgi::intersects(spatialRegion)); it != rTree->qend(); ++it)
    {

        box spatialRegionOfNode = it->first;
        int currnode = it->second.second;
        int isMbr = it->second.first;

        // cout << queryParam->queryNode << " " << currnode << endl;
        pair<int, int> currnodePair = it->second;
        if (!isMbr) {
            if (socialGraph->reachNode(node, currnode)) {
               return true;
            }
        }
        else
        {
            spatialMbrRelation LocationNode = spatialGraph->getLocation(currnode);
            if (get<0>(checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion)))
            {

                if (socialGraph->reachNode(node, currnode)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool strictSpatialFirstQuery(Graph* SocialGraph, bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics)
{
    int node = queryParam->queryNode;
    box spatialRegion = queryParam->spatialRegion;
    
    
    #ifdef STATISTICS
        Timer clock;
        clock.start();
    #endif
    vector<SpatialNode> possibleHits;
    rTree->query(bgi::intersects(spatialRegion), back_inserter(possibleHits));

    #ifdef STATISTICS
        double spatial_time = clock.stop();
        int reachability_test_counter = 0;
        double time_social = 0;
        clock.start();
    #endif

    for (vector<SpatialNode>::iterator iter = possibleHits.begin(); iter != possibleHits.end(); iter++)
    {
        #ifdef STATISTICS
            reachability_test_counter++;
        #endif
        if (SocialGraph->reachNode(node, iter->second))
        {
            #ifdef STATISTICS
                statistics->time_spatial = spatial_time;
                statistics->time_social = clock.stop();
                statistics->nodes_inside_query_range = possibleHits.size();
                statistics->number_of_reachability_tests = reachability_test_counter;
            #endif

            return true;
        }
    }
    #ifdef STATISTICS
        statistics->time_spatial = spatial_time;
        statistics->time_social = clock.stop();
        statistics->nodes_inside_query_range = possibleHits.size();
        statistics->number_of_reachability_tests = reachability_test_counter;
    #endif
    return false;
}

bool strictSpatialFirstQueryWithMbr(LocationMap* spatialGraph, Graph* socialGraph, bgi::rtree<SccWithMbr, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics)
{
    int node = queryParam->queryNode;
    box spatialRegion = queryParam->spatialRegion;

    vector<SccWithMbr> possibleHits;
    
    Timer clock;

    clock.start();
    rTree->query(bgi::intersects(spatialRegion), back_inserter(possibleHits));
    int reachability_test_counter = 0;

    vector<int> points_inside_query_window;

    for (vector<SccWithMbr>::iterator it = possibleHits.begin(); it != possibleHits.end(); it++)
    {
        box spatialRegionOfNode = it->first;
        int currnode = it->second.second;
        int isMbr = it->second.first;
        if (!isMbr) {
            points_inside_query_window.push_back(currnode);
        }
        else
        {
            spatialMbrRelation LocationNode = spatialGraph->getLocation(currnode);
            if (get<0>(checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion)))
            {
                points_inside_query_window.push_back(currnode);
            }
        }
    }
    double spatial_time = clock.stop();
    
    clock.start();
    for (vector<int>::iterator it = points_inside_query_window.begin(); it != points_inside_query_window.end(); it++)
    {
        reachability_test_counter++;
        if (socialGraph->reachNode(node, *it))
        {
            #ifdef STATISTICS
                statistics->time_spatial = spatial_time;
                statistics->time_social = clock.stop();
                statistics->nodes_inside_query_range = points_inside_query_window.size();
                statistics->number_of_reachability_tests = reachability_test_counter;
            #endif
            return true;
        }
    }
    #ifdef STATISTICS
        statistics->time_spatial = spatial_time;
        statistics->time_social = clock.stop();
        statistics->nodes_inside_query_range = points_inside_query_window.size();
        statistics->number_of_reachability_tests = reachability_test_counter;
    #endif
    
    return false;
}

bool spatialFirstQueryUsingBfl(Graph* SocialGraph, bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics)
{
 int node = queryParam->queryNode;
    box spatialRegion = queryParam->spatialRegion;
    for (auto it = rTree->qbegin(bgi::intersects(spatialRegion)); it != rTree->qend(); ++it)
    {
        cout << node << " => " << it->second << endl;
        int u,v;
        u = node;
        if (SocialGraph->NodeBelongsToSCC.count(node) != 0)
        {
            u = SocialGraph->NodeBelongsToSCC[node]; 
        }
        v = it->second;
        u = SocialGraph->nodeToBflIdentifier[u];
        v = SocialGraph->nodeToBflIdentifier[v]; 
        if (run_single_bfl_query(u,v)) 
        {
            return true;
        }
    }
    return false;
};

bool spatialFirstQueryUsingBflWithMbr(LocationMap* spatialGraph, Graph* socialGraph, bgi::rtree<SccWithMbr, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics)
{
int node = queryParam->queryNode;
    box spatialRegion = queryParam->spatialRegion;

    for (auto it = rTree->qbegin(bgi::intersects(spatialRegion)); it != rTree->qend(); ++it)
    {
        box spatialRegionOfNode = it->first;
        int currnode = it->second.second;
        int isMbr = it->second.first;
        pair<int, int> currnodePair = it->second;
        // CHECK FOR POINT

        int u,v;
        u = node;
        if (socialGraph->NodeBelongsToSCC.count(node) != 0)
        {
            u = socialGraph->NodeBelongsToSCC[node]; 
        }
        v = currnode;
        u = socialGraph->nodeToBflIdentifier[u];
        v = socialGraph->nodeToBflIdentifier[v]; 
        if (!isMbr) {
            if (run_single_bfl_query(u,v)) {
               return true;
            }
        }
        else
        {
            spatialMbrRelation LocationNode = spatialGraph->getLocation(currnode);
            if (get<0>(checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion)))
            {
                if (run_single_bfl_query(u,v)) {
                    return true;
                }
            }
        }
    }
    return false;
};

bool strictSpatialFirstQueryUsingBfl(Graph* SocialGraph, bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics)
{
    int node = queryParam->queryNode;
    box spatialRegion = queryParam->spatialRegion;
    vector<SpatialNode> possibleHits;
    
    
    #ifdef STATISTICS
        Timer clock;
        clock.start();
    #endif
    
    rTree->query(bgi::intersects(spatialRegion), back_inserter(possibleHits));

    #ifdef STATISTICS
        double spatial_time = clock.stop();
        int reachability_test_counter = 0;
        double time_social = 0;
        clock.start();
    #endif


    for (vector<SpatialNode>::iterator iter = possibleHits.begin(); iter != possibleHits.end(); iter++)
    {
        #ifdef STATISTICS
            reachability_test_counter++;
        #endif
        
        int u,v;
        u = node;
        if (SocialGraph->NodeBelongsToSCC.count(node) != 0)
        {
            u = SocialGraph->NodeBelongsToSCC[node]; 
        }
        

        v = iter->second;
        u = SocialGraph->nodeToBflIdentifier[u];
        v = SocialGraph->nodeToBflIdentifier[v];
        if (node != v){
            if (run_single_bfl_query(u, v))
                {
                    #ifdef STATISTICS
                        statistics->time_spatial = spatial_time;
                        statistics->time_social = clock.stop();
                        statistics->nodes_inside_query_range = possibleHits.size();
                        statistics->number_of_reachability_tests = reachability_test_counter;
                    #endif

                    return true;
                }
        } 
        
    }
    #ifdef STATISTICS
        statistics->time_spatial = spatial_time;
        statistics->time_social = clock.stop();
        statistics->nodes_inside_query_range = possibleHits.size();
        statistics->number_of_reachability_tests = reachability_test_counter;
    #endif
    return false;
};

bool strictSpatialFirstQueryUsingBflWithMbr(LocationMap* spatialGraph, Graph* socialGraph, bgi::rtree<SccWithMbr, bgi::linear<16>>* rTree, vector<queryParameter>::iterator queryParam, spatialFirstResult* statistics)
{
     int node = queryParam->queryNode;
    box spatialRegion = queryParam->spatialRegion;

    vector<SccWithMbr> possibleHits;
    
    Timer clock;

    clock.start();
    rTree->query(bgi::intersects(spatialRegion), back_inserter(possibleHits));
    int reachability_test_counter = 0;

    vector<int> points_inside_query_window;

    for (vector<SccWithMbr>::iterator it = possibleHits.begin(); it != possibleHits.end(); it++)
    {
        box spatialRegionOfNode = it->first;
        int currnode = it->second.second;
        int isMbr = it->second.first;
        if (!isMbr) {
            points_inside_query_window.push_back(currnode);
        }
        else
        {
            spatialMbrRelation LocationNode = spatialGraph->getLocation(currnode);
            if (get<0>(checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion)))
            {
                points_inside_query_window.push_back(currnode);
            }
        }
    }
    double spatial_time = clock.stop();
    
    clock.start();
    for (vector<int>::iterator it = points_inside_query_window.begin(); it != points_inside_query_window.end(); it++)
    {
        reachability_test_counter++;

                
        int u,v;
        u = node;
        if (socialGraph->NodeBelongsToSCC.count(node) != 0)
        {
            u = socialGraph->NodeBelongsToSCC[node]; 
        }
        v = *it;
        u = socialGraph->nodeToBflIdentifier[u];
        v = socialGraph->nodeToBflIdentifier[v]; 
        if (run_single_bfl_query(u,v))
        {
            #ifdef STATISTICS
                statistics->time_spatial = spatial_time;
                statistics->time_social = clock.stop();
                statistics->nodes_inside_query_range = points_inside_query_window.size();
                statistics->number_of_reachability_tests = reachability_test_counter;
            #endif
            return true;
        }
    }
    #ifdef STATISTICS
        statistics->time_spatial = spatial_time;
        statistics->time_social = clock.stop();
        statistics->nodes_inside_query_range = points_inside_query_window.size();
        statistics->number_of_reachability_tests = reachability_test_counter;
    #endif
    
    return false;
};
