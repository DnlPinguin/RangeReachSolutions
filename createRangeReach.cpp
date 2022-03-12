#include "main.h"

string getFileName(){
	cout << "Enter filename:";
	string input;
	cin >> input;
	return input;
}

float getMAX_RMBR(){
    cout << "Enter float to specify when R-Vertices will be degraded to B-Vertex:";
	string input;
	cin >> input;
	return stof(input);
}

int getMAX_REACH_GRIDS(){
    cout << "Enter amount of grids per g_vertex before degrading to r_vertex:";
	string input;
	cin >> input;
	return stoi(input);
}

int getLayer(){
    cout << "Enter amount of layers: (e.g: \n1 equals 1 \n2 equals 2x2=4-1 \n3 equals 4x4=16-4-1 \n8 equals 128x128=16384 \n";
	string input;
	cin >> input;
	return stoi(input);
}

int getMergeCount(){
    cout << "Enter amount of grids necessary to be merged into lower level:";
	string input;
	cin >> input;
	return stoi(input);
}

int main(int argc, char **argv){
    string filename;
	int MAX_REACH_GRIDS, LAYER, MERGE_COUNT;
    float MAX_RMBR;
	if (argc == 6)
    {
        filename = argv[1];
        MAX_RMBR = stof(argv[2]);
        MAX_REACH_GRIDS = stof(argv[3]);
        LAYER = stof(argv[4]);
        MERGE_COUNT = stof(argv[5]);
	} else 
    {
        filename = getFileName();
        MAX_RMBR = getMAX_RMBR();
        MAX_REACH_GRIDS = getMAX_REACH_GRIDS();
        LAYER = getLayer();
        MERGE_COUNT = getMergeCount();
	}

    Graph SocialGeoGraph;
    LocationMap LocationGraph;
    RangeReachVertex RangeReach;

    SocialGeoGraph.readReducedGraph("./data/processed/" + filename + "_reduced_scheme");
    SocialGeoGraph.readSuperConnectedComponents("./data/processed/" + filename + "_strongly_connected_components");
    LocationGraph.readFileForMap("./data/processed/" + filename + "_reduced_spatial_data");
    
	RangeReach.maximumMBR = MBR(LocationGraph.MinMaxCorners[0],LocationGraph.MinMaxCorners[1],LocationGraph.MinMaxCorners[2],LocationGraph.MinMaxCorners[3]);
	RangeReach.maximumMBR.print();
    
    RangeReach.createGridField(LAYER);

    cout << "create SPA-Graph structure  \n";
    RangeReach.createGVertex(&SocialGeoGraph, &LocationGraph, MAX_RMBR, MAX_REACH_GRIDS, LAYER);
    
    if (MERGE_COUNT > 1 ){
        RangeReach.MergeGVertex(MERGE_COUNT);
    }
    RangeReach.createRVertex(MAX_REACH_GRIDS, &LocationGraph);

    float totalSizeOfArea = fabs(RangeReach.maximumMBR.xMax - RangeReach.maximumMBR.xMin) * (RangeReach.maximumMBR.yMax - RangeReach.maximumMBR.yMin);
    RangeReach.createBVertex(MAX_RMBR, totalSizeOfArea);

    // RangeReach.printBVertex();
    // RangeReach.printRVertex();
    // RangeReach.printGVertex();
    // RangeReach.printAllGridLayers();
    RangeReach.checkVertexCorrectnes();
    RangeReach.writeAttributesToFile(filename);

    return 0;
}