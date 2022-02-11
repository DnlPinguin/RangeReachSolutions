#include "main.h"

string getFileName(){
	cout << "Enter filename:";
	string input;
	cin >> input;
	return input;
}

float getMAX_RMBR(){
    cout << "Enter MAX_RMBR:";
	string input;
	cin >> input;
	return stof(input);
}

int getMAX_REACH_GRIDS(){
    cout << "Enter MAX REACH GRIDS:";
	string input;
	cin >> input;
	return stoi(input);
}

int getLayer(){
    cout << "Enter amount of grids in top layer:";
	string input;
	cin >> input;
	return stoi(input);
}

int getMergeCount(){
    cout << "Enter merge count:";
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

    loadFile(filename, &SocialGeoGraph);
    loadVanillaGeoFileData(filename, &LocationGraph);
    SocialGeoGraph.createSCCsUsingTarjans();
    SocialGeoGraph.writeSuperConnectedComponents(filename);
    SocialGeoGraph.writeReducedGraph(filename);
    LocationGraph.createMbrs(&SocialGeoGraph.SuperConnectedComponents);
	LocationGraph.writeMapToFile("./data/processed/" + filename + "_reduced_spatial_data");
	RangeReach.maximumMBR = MBR(0,0,1,1);
    RangeReach.createGridField(LAYER);
	RangeReach.CreateRangeReachVertex(&SocialGeoGraph, &LocationGraph, MAX_RMBR, MAX_REACH_GRIDS, LAYER);
    if (MERGE_COUNT != 0 ){
        RangeReach.MergeGVertex(MERGE_COUNT);
    }
    RangeReach.createRVertex(MAX_REACH_GRIDS, &LocationGraph);

    // RangeReach.printBVertex();
    RangeReach.printGVertex();
    // RangeReach.printRVertex();
    // RangeReach.printAllGridLayers();
    RangeReach.writeAttributesToFile(filename);

    return 0;
}