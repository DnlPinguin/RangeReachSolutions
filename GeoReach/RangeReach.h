#ifndef RANGEREACH_H
#define RANGEREACH_H


#include "../Graph/graph.h"
#include "../Structures/boostTypes.h"
#include "../Spatial/MBR.h"
#include <map>
#include <unordered_map>
#include <fstream>
#include <iostream>






class RangeReachVertex {
public: 
	// Vertices of the Graph
	vector<int> V;
	// Edges of the Graph
	unordered_map<int, vector<int>> E;
	// Vertices containing spatial data
	unordered_map<int, Location> Vs;

	unordered_map<int, spatialMbrRelation> VsWithSCC;

	unordered_map<int, vector<int>> StronglyConnectedComponent;

	unordered_map<int, vector<int>> Nodes_In_G_Vertex;


	// Boolean map to Check wether a node is able to reach a node with spatial information
	unordered_map<int, bool> B_Vertex;
	
	// MBR of all nodes reachable by this node
	unordered_map<int, MBR> R_Vertex;
	
	// Vector of spatial Grids reachable by this node
	unordered_map<int, unordered_set<int>> G_Vertex;


	map<int, MBR> spatialGridBuffer;
	map<int, MBR> spatialGrid;


	map<int,map<int, MBR>> gridLayers;
	
	// Maximum Grids spanned by all spatial nodes
	MBR maximumMBR;



	coordinates chunkStepX, chunkStepY;

	unordered_map<int, bool> alreadyChecked;
	

	void writeAttributesToFile(string superFile);

	void readAttributesFromFile(string superFile);
	
	void createGridField(int layers);

	//Builds the RangeReachVertex according to the social and spatial Data;
	RangeReachVertex();

	void CreateRangeReachVertex(Graph* socialGraph, LocationMap* locationGraph, float MAX_RMBR, float MAX_REACH_GRIDS, int layers);

	bool SpaReachQuery(int node, box queryWindow, Graph* socialGraph, LocationMap* spatialGraph, int layers);

	void MergeGVertex(int MERGE_COUNT);
	void createRVertex(int MAX_REACH_GRIDS, LocationMap* spatialGraph);

	MBR getGridFieldById(int id);
	vector<int> getLowerLevelsGridsInside(int currentLayer, MBR grid);

	//Gets the spatial Grid field of the location
	int getSpatialGridField(Location nodeLocation, int layers);

	void getAllSpatialGridsInArea(MBR queryArea, int layers,vector<int>* reachableGrids);

	vector<int> getAllChildrens(int node);
	vector<int> getAllChildrensWithScc(int node, Graph* G);

	//Performs the rangeReach query for a node and a given spatial region
	bool rangeReach(int node, box spatialRegion);
	bool rangeReachSCC(int node, box spatialRegion);
#pragma region PRINT STATEMENTS

	void printAttributes();

	void printGridField();

	void printAllGridLayers();

	void printSCCAttributes();
	
	void printBVertex();
	
	void printRVertex();
	
	void printGVertex();

	void printReachableNodes();


#pragma endregion	
};
#endif 