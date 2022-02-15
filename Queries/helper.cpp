#include "helper.h"

bool isContained(box rectangleOne, box rectangleTwo) {
    // Left Bottom Corner
    int min_x = rectangleOne.min_corner().get<0>();
    int min_y = rectangleOne.min_corner().get<1>();
    // Right Upper Corner
    int max_x = rectangleOne.max_corner().get<0>();
    int max_y = rectangleOne.max_corner().get<1>();

    int two_min_x = rectangleTwo.min_corner().get<0>();
    int two_min_y = rectangleTwo.min_corner().get<1>();
    // Right Upperss Corner
    int two_max_x = rectangleTwo.max_corner().get<0>();
    int two_max_y = rectangleTwo.max_corner().get<1>();


    if (min_x > two_min_x && max_x < two_max_x && min_y > two_max_y && max_y < two_max_y)
    {
        return true;
    }
    return false;
    
}

bool checkIfRectangleIsContainedInRegion(box rectangle, box spatialRegion) {
    cout << "queryWindow ->" << spatialRegion.min_corner().get<0>() << " " << spatialRegion.min_corner().get<1>() << " " << spatialRegion.max_corner().get<0>() << " " << spatialRegion.max_corner().get<1>() << endl;
    return boost::geometry::within(rectangle, spatialRegion);
}

bool checkIfOneEdgeIsContainedInRectangle(box rectangle, box spatialRegion)
{
    // Left Bottom Corner
    coordinates left_x = rectangle.min_corner().get<0>();
    coordinates bottom_y = rectangle.min_corner().get<1>();

    // Right Upper Corner
    coordinates right_x = rectangle.max_corner().get<0>();
    coordinates top_y = rectangle.max_corner().get<1>();


    coordinates min_x = spatialRegion.min_corner().get<0>();
    coordinates min_y = spatialRegion.min_corner().get<1>();
    coordinates max_x = spatialRegion.max_corner().get<0>();
    coordinates max_y = spatialRegion.max_corner().get<1>();

    // cout << "Location of Rectangle:" << left_x << " " << bottom_y << " " << right_x << " " << top_y << endl;
    // cout << "location of spatialregion: " << min_x << " " << min_y << " " << max_x << " " << max_y << endl;
    
    // Left Bottom Corner
    point left_bottom_corner(left_x, bottom_y);
    // Right Upper Corner
    point right_upper_corner(right_x, top_y);
    // Left Upper Corner
    point left_upper_corner(left_x, top_y);
    // Right Bottom Corner
    point right_bottom_corner(right_x, bottom_y);

    box edge = box (left_bottom_corner, right_bottom_corner);
    cout << "Bottom Edge ->" << edge.min_corner().get<0>() << " " << edge.min_corner().get<1>() << " " << edge.max_corner().get<0>() << " " << edge.max_corner().get<1>() << endl;
    if (left_x >= min_x && right_x <= max_x && bottom_y >= min_y && bottom_y <= max_y)
    {
        cout << " Bottom Edge hit" << endl;
        return true; 
    }

    edge = box(left_bottom_corner, left_upper_corner);
    cout << "Left Edge ->" << edge.min_corner().get<0>() << " " << edge.min_corner().get<1>() << " " << edge.max_corner().get<0>() << " " << edge.max_corner().get<1>() << endl;
    if (left_x >= min_x && left_x <= max_x && bottom_y >= min_y && top_y <= max_y)
    {
        cout << " Left Edge hit" << endl;
        return true; 
    }

    edge = box(right_bottom_corner, right_upper_corner);
    cout << "Right Edge ->" << edge.min_corner().get<0>() << " " << edge.min_corner().get<1>() << " " << edge.max_corner().get<0>() << " " << edge.max_corner().get<1>() << endl;
    if (right_x >= min_x && right_x <= max_x && bottom_y >= min_y && top_y <= max_y)
    {
        cout << " Right Edge hit" << endl;
        return true;
    }
    edge = box(left_upper_corner, right_upper_corner);
    cout << "Top Edge ->" << edge.min_corner().get<0>() << " " << edge.min_corner().get<1>() << " " << edge.max_corner().get<0>() << " " << edge.max_corner().get<1>() << endl;
    if (left_x >= min_x && right_x <= max_x && top_y >= min_y && top_y <= max_y)
    {
        cout << " Top Edge hit" << endl;
        return true;
    }

    return false;

}

int computeCardinality(bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, box spatialRegion)
{
    vector<SpatialNode> possibleHits;
    rTree->query(bgi::intersects(spatialRegion), back_inserter(possibleHits));
    return possibleHits.size();
}

void getNodesInsideSpatialField(bgi::rtree<SpatialNode, bgi::linear<16>>* rTree, box spatialRegion, vector<int>* nodes, double* time){
    vector<SpatialNode> possibleHits;
    rTree->query(bgi::intersects(spatialRegion), back_inserter(possibleHits));
    for (vector<SpatialNode>::iterator iter = possibleHits.begin(); iter != possibleHits.end(); iter++)
        nodes->push_back(iter->second);
}

tuple<bool,int> checkIfNodeIsInSpatialRegion(bool isMbr, vector<coordinates> LocationNode, box spatialRegion){
    int counter = 0; 
    if (isMbr) 
    {
        box mbr = box(point(LocationNode[0], LocationNode[1]), point(LocationNode[2], LocationNode[3]));
        cout << "mbr ->" << mbr.min_corner().get<0>() << " " << mbr.min_corner().get<1>() << " " << mbr.max_corner().get<0>() << " " << mbr.max_corner().get<1>() << endl;
        counter++;
        if (checkIfRectangleIsContainedInRegion(mbr, spatialRegion)) 
        {
            cout << " Rectangle is fully contained in region." << endl;
            return make_tuple(true, counter);
        }
        counter++;
        if (checkIfOneEdgeIsContainedInRectangle(mbr, spatialRegion)) 
        {
            cout << " One Edge is fully contained in region." << endl;
            return make_tuple(true, counter);
        }
        counter++;
        if (boost::geometry::intersects(spatialRegion, mbr)) 
        {
            //Check For expandend Nodes
            // cout << "Need to expand the component." << endl;
            for (int i = 4; i != LocationNode.size(); i++) 
            {
                box mbr = box(point(LocationNode[i], LocationNode[(i + 1)]), point(LocationNode[i], LocationNode[(i + 1)]));
                counter++;
                if (boost::geometry::intersects(spatialRegion, mbr))
                {
                    return make_tuple(true, counter);
                }
            }
        }
    }
    else 
    {
        box mbr = box(point(LocationNode[0], LocationNode[1]), point(LocationNode[0], LocationNode[1]));
        counter++;
        if (boost::geometry::intersects(spatialRegion, mbr))
        {
            //cout << " Normal point is contained in spatial Region." << endl;
            return make_tuple(true, counter);
        }
    }

    return make_tuple(false, counter);
}