#include"rTree.h"

typedef int node;

rTreeLines buildThreeDimensionalRtree(Graph* HybridGraph, LocationMap* LocationGraph) {
    cout << "Build R-Tree with lines \n";
    rTreeLines rtree;
    int lineCounter = 0;
    for (unordered_map<int,Location>::iterator iter = LocationGraph->LocationScheme.begin(); iter != LocationGraph->LocationScheme.end(); iter++)
    {
        int node = iter->first;
        if (HybridGraph->NodeBelongsToSCC.count(iter->first) != 0)
        {   
            node = HybridGraph->NodeBelongsToSCC[iter->first];
        }
        for (IntervalScheme it : HybridGraph->IntervalSchemeGraphMap[node])
        {
            cout << iter->second.x << " " << iter->second.y << " " << it.pre << endl;
            threeDimLine line(
                threeDimPoint(iter->second.x, iter->second.y, float(it.pre)),
                threeDimPoint(iter->second.x, iter->second.y, float(it.post))
            );
            lineCounter++;
            rtree.insert(make_pair(line, iter->first));
        }
    }
    cout << "R-Tree completed: " << lineCounter << " lines added.\n";
    return rtree;
}

rTreeCubes buildThreeDimensionalRtreeWithCuboids(Graph* HybridGraph, LocationMap* LocationGraph) {
    rTreeCubes rtree;

    unordered_map<int, spatialMbrRelation> SpatialData = LocationGraph->returnMap();
    int cube_counter = 0;
    for (unordered_map<int, spatialMbrRelation>::iterator iter = SpatialData.begin(); iter != SpatialData.end(); iter++)
    {
        spatialMbrRelation Locations = iter->second;
        vector<IntervalScheme> intervalData = HybridGraph->getIntervalSchemeNode(iter->first);
        coordinates x_1 = Locations.spatialData[0];
        coordinates y_1 = Locations.spatialData[1];
        coordinates x_2 = Locations.spatialData[2];
        coordinates y_2 = Locations.spatialData[3];

        for (IntervalScheme it : intervalData) {
            int z_1 = it.pre;
            int z_2 = it.post;
            cube_counter++;
            plane cuboid(threeDimPoint(x_1, y_1, z_1), threeDimPoint(x_2, y_2, z_2));
            rtree.insert(make_pair(cuboid, iter->first));
        }
    }
    cout << "\tR-Tree completed." << cube_counter << " cubes added.\n";
    return rtree;
}

rTreePoints buildThreeDimensionalRtreeWithPoints(Graph* HybridGraph, LocationMap* LocationGraph, Graph* SocialGraph){
    cout << "Build reverse R-Tree with points \n";
    rTreePoints rtree;
    int lineCounter = 0;
    for (unordered_map<int, Location>::iterator iter = LocationGraph->LocationScheme.begin(); iter != LocationGraph->LocationScheme.end(); iter++)
    {
        Location pos = iter->second;
        int spatialNode = iter->first;
        if (SocialGraph->NodeBelongsToSCC.count(spatialNode) != 0){
            spatialNode = SocialGraph->NodeBelongsToSCC[iter->first];
        }

        for (IntervalScheme it : HybridGraph->IntervalSchemeGraphMap[spatialNode])
        {
            cout << "Insert Point: " << pos.x << " " << pos.y << " " << it.post << " " << spatialNode << endl;
            lineCounter++;
            rtree.insert(threeDimPoint(pos.x, pos.y, it.post));
        }
    }

    cout << "R-Tree completed: " << lineCounter << " lines added.\n";
    return rtree;
}

rTreeCubes buildThreeDimesionalRTreeWithPlanes(Graph* HybridGraph, LocationMap* LocationGraph, Graph* SocialGraph){
    rTreeCubes rtree;
    unordered_map<int, spatialMbrRelation> SpatialData = LocationGraph->returnMap();
    int cube_counter = 0;
    for (unordered_map<int, spatialMbrRelation>::iterator iter = SpatialData.begin(); iter != SpatialData.end(); iter++)
    {
        spatialMbrRelation Locations = iter->second;
        int postOrder  = HybridGraph->getPostOrderOfNode(iter->first);
        coordinates x_1 = Locations.spatialData[0];
        coordinates y_1 = Locations.spatialData[1];
        coordinates x_2 = Locations.spatialData[2];
        coordinates y_2 = Locations.spatialData[3];

        // Locations.print();
        // cout << "postorder" << postOrder << endl;
        plane cuboid(threeDimPoint(x_1, y_1, postOrder), threeDimPoint(x_2, y_2, postOrder));
        rtree.insert(make_pair(cuboid, iter->first));
        cube_counter++;
    }
    cout << cube_counter << " cubes added. R-Tree completed. \n";
    return rtree;
}


rTreePlanes* buildTwoDimensionalRtree(LocationMap* LocationGraph) {
    rTreePlanes* rtree = new rTreePlanes;
    for (unordered_map<int, spatialMbrRelation>::iterator iter = LocationGraph->Map.begin(); iter != LocationGraph->Map.end() ; iter++)
    {
        spatialMbrRelation Locations = iter->second;

        if (!Locations.isMbr )
        {
            rtree->insert(make_pair(point(Locations.spatialData[0], Locations.spatialData[1]), iter->first));
        }
        else 
        {
            for (int i = 4; i < Locations.spatialData.size(); i = i + 2)
            {
                rtree->insert(make_pair(point(Locations.spatialData[i], Locations.spatialData[i+1]), iter->first));
            }
        }
    }
    return rtree;
}

rTreeSccPlanes* buildTwoDimensionalRtreeWithMbr(LocationMap* spatialGraph) {
    rTreeSccPlanes* rtree = new rTreeSccPlanes;
    cout << "Build 2d R-Tree with Lines \n";
    unordered_map<int, spatialMbrRelation> SpatialData = spatialGraph->returnMap();
    for (unordered_map<int, spatialMbrRelation>::iterator iter = SpatialData.begin(); iter != SpatialData.end(); iter++)
    {   
        box Mbr(point(iter->second.spatialData[0],iter->second.spatialData[1]), point(iter->second.spatialData[2], iter->second.spatialData[3]));
        rtree->insert(make_pair(Mbr, make_pair(iter->second.isMbr, iter->first)));
    }
    cout << "R-Tree completed\n";
    return rtree;
}