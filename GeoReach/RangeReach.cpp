#include "RangeReach.h"


/*
Checks whether the sspatial grid is intersecting with the spatial region. If not we can say the node is not reaching any nodes lying the the spatial target region
*/
bool intersects(MBR spatialGrid, box queryWindow) {
	coordinates queryWindowMinX = queryWindow.min_corner().get<0>();
	coordinates queryWindowMinY = queryWindow.min_corner().get<1>();
	coordinates queryWindowMaxX = queryWindow.max_corner().get<0>();
	coordinates queryWindowMaxY = queryWindow.max_corner().get<1>();

 	if (spatialGrid.xMax < queryWindowMinX || spatialGrid.xMin > queryWindowMaxX) {
		return false;
	}
	// If one rectangle is above other 
	if (spatialGrid.yMax < queryWindowMinY || spatialGrid.yMin > queryWindowMaxY) {
		return false;
	}

	return true;
}

/*
 Checks if the spatial region is overlapping the spatial grid returned by the vertex. If so, the whole grid lies inside the spatial region and the node is automatically
in this region too. 
*/
bool overlaps(MBR spatialGrid, box queryWindow) {
	if (spatialGrid.xMin >= queryWindow.min_corner().get<0>()
		&& spatialGrid.yMin >= queryWindow.min_corner().get<1>()
		&& spatialGrid.xMax <= queryWindow.max_corner().get<0>()
		&& spatialGrid.yMax <= queryWindow.max_corner().get<1>()
	)
	{
		return true;
	}
	return false;
}

void printSpatialGridCells(map<int, MBR> spatialGrid) {
	cout << "Print Statial Grids" << endl;
	for (auto i : spatialGrid) {
		cout << "SpatialGrid: " << i.first << " -> ";
		i.second.print();
	}
	cout << endl;
}

RangeReachVertex::RangeReachVertex() 
{
};

/**
 * @brief Create default empty Grid Cell. Layer 0 has LAYERS x LAYERS dimensions
 * E.g Layers Parameter 4 creates
 * Layer 0 : 4x4 = 16 fields 
 * Layer 1 : 2x2 = 4 fields
 * Layer 2 : 1x1 = 1 field
 */
void RangeReachVertex::createGridField(int LAYERS) 
{
	cout << "Create Grid Field for " << LAYERS << " layers." << endl;
	int layerLevel = 0;
	this->maximumMBR.print();
	int layerCounter = static_cast<float>(LAYERS);
	this->chunkStepX = abs(maximumMBR.xMax - maximumMBR.xMin) / layerCounter;
	this->chunkStepY = abs(maximumMBR.yMax - maximumMBR.yMin) / layerCounter;

	int totalGridCounter = 0;
	int gridPerLayerCounter = 0;
	while(layerCounter > 0){
		int gridsPerLayer = pow(2, layerCounter);
		// int gridsPerRow = sqrt(gridsPerLayer);

		int gridsPerRow = gridsPerLayer / 2;
		float layerChunkStepX = abs(maximumMBR.xMax - maximumMBR.xMin) / gridsPerRow;
		float layerChunkStepY = abs(maximumMBR.yMax - maximumMBR.yMin) / gridsPerRow;
		gridPerLayerCounter = 0;
		for (int i = 0; i < gridsPerRow; i++) {
			for (int j = 0; j < gridsPerRow; j++) {

				Location gridBox(maximumMBR.xMin + (i)*layerChunkStepX, maximumMBR.yMin + j * layerChunkStepY);
				Location gridBoxUpper(maximumMBR.xMin + (i + 1) * layerChunkStepX, maximumMBR.yMin + (j + 1) * layerChunkStepY);
				MBR spatialGridCell(gridBox);
				spatialGridCell.insertLoc(gridBoxUpper);
				spatialGrid[j + i * gridsPerRow + totalGridCounter] = spatialGridCell;
				spatialGridBuffer[j + i * gridsPerRow + totalGridCounter] = spatialGridCell;
				gridPerLayerCounter++;
			}
		}
		totalGridCounter += gridPerLayerCounter; 
		this->gridLayers[layerLevel] = spatialGridBuffer;
		spatialGridBuffer.clear();
		layerLevel++;
		layerCounter--;
	}
};

void RangeReachVertex::createGVertex(Graph* socialGraph, LocationMap* locationGraph, float MAX_RMBR = 1, float MAX_REACH_GRIDS = 3, int LAYERS=4) {
	cout << "Create G Vertex" << endl;
	vector<int> topologicalOrderedVertices;
	
	socialGraph->topologicalSort(&topologicalOrderedVertices);

	vector<coordinates> maxMBR = locationGraph->getMinMaxCorners();

	this->maximumMBR = MBR(maxMBR[0], maxMBR[1], maxMBR[2], maxMBR[3]);
	// this->maximumMBR = MBR(0, 0, 1, 1);


	unordered_map<int, vector<int>> spatialGridMap;
	
	cout << "Create Grid Ids" << endl;
	int counter = 0;
	for (unordered_map<int, spatialMbrRelation>::iterator iter = locationGraph->Map.begin();iter != locationGraph->Map.end(); iter++){
		counter++;
		if (counter % 15000 == 0){
            cout << counter <<  " / " << locationGraph->Map.size() <<  "    " << (((float)counter / (float)locationGraph->Map.size()) * 100)  << "%" << endl;
        }

		if (iter->second.isMbr){
			spatialGridMap[iter->first] = {};
			for (int i = 4; i < iter->second.spatialData.size() - 1; i += 2){
				Location pos = Location(iter->second.spatialData[i], iter->second.spatialData[i+1]);
				spatialGridMap[iter->first].push_back(getSpatialGridField(pos, LAYERS));

			}
		}
		else{
			Location pos = Location(iter->second.spatialData[0], iter->second.spatialData[1]);
			spatialGridMap[iter->first] =  {getSpatialGridField(pos, LAYERS)};
		}
	}

	int size = topologicalOrderedVertices.size();
	counter = 0;
	cout << "Calculate G_Vertex" << endl;
	for (int iter = size; iter >= 0; iter--){
		int currentNode = topologicalOrderedVertices[iter];	
		bool currentNodeReachesSpatial = false;
        if (counter % 15000 == 0){
            cout << counter <<  " / " << size <<  "    " << (((float)counter / (float)size) * 100)  << "%" << endl;
        }
		for (int reachableNode : socialGraph->GraphScheme[currentNode]){

			if (locationGraph->existLocation(reachableNode)){
				currentNodeReachesSpatial = true;
				spatialMbrRelation spatialStructure = locationGraph->getLocation(reachableNode);
				vector<coordinates> pointsInsideScc = spatialStructure.spatialData;


				for (int i : spatialGridMap[reachableNode]){
					if (G_Vertex[currentNode].size() <= MAX_REACH_GRIDS){
						G_Vertex[currentNode].insert(i);
					} else{
						break;
					}
				}

				if (spatialStructure.isMbr)
				{
					for (int i = 4; i < pointsInsideScc.size() - 1; i += 2){
						Location pos = Location(pointsInsideScc[i], pointsInsideScc[i+1]);
						if (MBRInsideGVertex.count(currentNode) != 0){
							MBRInsideGVertex[currentNode].insertLoc(pos);
						} else {
							MBRInsideGVertex[currentNode] = MBR(pos);
						}
					}
				}else{
					Location pos = Location(pointsInsideScc[0], pointsInsideScc[1]);
					if (MBRInsideGVertex.count(currentNode) != 0){
							MBRInsideGVertex[currentNode].insertLoc(pos);
						} else {
							MBRInsideGVertex[currentNode] = MBR(pos);
						}
				}

				// if (socialGraph->SuperConnectedComponents.count(reachableNode)){

				// 	// for (int i = 4; i < pointsInsideScc.size() - 1; i += 2){
				// 	// 	Location pos = Location(pointsInsideScc[i], pointsInsideScc[i+1]);
				// 	// 	// G_Vertex[currentNode].insert(getSpatialGridField(pos, LAYERS));

				// 	// 	G_Vertex[currentNode].insert(spatialGridMap[reachableNode].begin(), spatialGridMap[reachableNode].end());

				// 	// 	// Locations_Inside_G_Vertex[currentNode].push_back(pos);
				// 	// 	if (MBRInsideGVertex.count(currentNode) != 0){
				// 	// 		MBRInsideGVertex[currentNode].insertLoc(pos);
				// 	// 	} else {
				// 	// 		MBRInsideGVertex[currentNode] = MBR(pos);
				// 	// 	}
				// 	// }

				// } else {
				// 	Location pos = Location(pointsInsideScc[0], pointsInsideScc[1]);
				// 	// G_Vertex[currentNode].insert(getSpatialGridField(pos, LAYERS));
				// 	G_Vertex[currentNode].insert(spatialGridMap[reachableNode].begin(), spatialGridMap[reachableNode].end());

				// 	// Locations_Inside_G_Vertex[currentNode].push_back(pos);

				// 	if (MBRInsideGVertex.count(currentNode) != 0){
				// 			MBRInsideGVertex[currentNode].insertLoc(pos);
				// 	} else {
				// 		MBRInsideGVertex[currentNode] = MBR(pos);
				// 	}
				// }
				if (this->G_Vertex.count(reachableNode) > 0){
					currentNodeReachesSpatial = true;
					for (int vertex : G_Vertex[reachableNode]){
						if (G_Vertex[currentNode].size() <= MAX_REACH_GRIDS){
							G_Vertex[currentNode].insert(vertex);
						}else{
							break;
						}
					}

					if (MBRInsideGVertex.count(currentNode) != 0){
						MBRInsideGVertex[currentNode].insertMBR(MBRInsideGVertex[reachableNode]);
					} else {
						MBRInsideGVertex[currentNode] = MBRInsideGVertex[reachableNode];
					}
				}
			}
		}
		if (!currentNodeReachesSpatial){
			B_Vertex[currentNode] = false;
		} 
	}

	return;
}

MBR RangeReachVertex::getGridFieldById(int id){
	map<int,map<int, MBR>>::iterator layerIterator = this->gridLayers.begin();
	for (layerIterator; layerIterator != this->gridLayers.end(); layerIterator++){
		int currentLayer = layerIterator->first;
		map<int, MBR>::iterator gridIterator = this->gridLayers[currentLayer].begin();
		for (gridIterator; gridIterator != this->gridLayers[currentLayer].end(); gridIterator++){
			if (id == gridIterator->first)
				return gridIterator->second;
		}	
	}
}

vector<int> RangeReachVertex::getLowerLevelsGridsInside(int currentLayer, MBR higherLevelGrid){
	int lowerLayer = currentLayer-1;
	map<int, MBR>::iterator gridIterator = this->gridLayers[lowerLayer].begin();

	vector<int> gridIds; 

	for (gridIterator; gridIterator != this->gridLayers[lowerLayer].end(); gridIterator++){
		int gridId = gridIterator->first;
		MBR grid = gridIterator->second;
		if (higherLevelGrid.overlaps(grid)){
			gridIds.push_back(gridId);
		}
	}
	return gridIds;
}

void RangeReachVertex::MergeGVertex(int MERGE_COUNT){
	cout << "Merge G-Vertex" << endl;

	map<int,map<int, MBR>>::iterator layerIterator = this->gridLayers.begin();
	layerIterator++;

	for (layerIterator; layerIterator != this->gridLayers.end(); layerIterator++){
		int currentLayer = layerIterator->first;
		map<int, MBR>::iterator gridIterator;
		for (gridIterator = this->gridLayers[currentLayer].begin(); gridIterator != this->gridLayers[currentLayer].end(); gridIterator++){
			int gridId = gridIterator->first;
			MBR grid = gridIterator->second;
			vector<int> lowerLevelGrids = this->getLowerLevelsGridsInside(currentLayer, grid);
			for (unordered_map<int,unordered_set<int>>::iterator iter = G_Vertex.begin(); iter != G_Vertex.end(); iter++) {
				int sameGrids = 0;
				vector<int> sameGridIds;

					
				for (int gVertexGridId : iter->second) {
					for (int lowerGridId : lowerLevelGrids)
					{
						if (gVertexGridId == lowerGridId){
							sameGrids ++;
							sameGridIds.push_back(gVertexGridId);
						}
					}
				}
				if (sameGrids >= MERGE_COUNT){

					unordered_set<int> mergedGrids;
					for (int gVertexGridId : iter->second) {
						bool duplicateFound = false;
						for (int duplicateGrid : sameGridIds)
						{
							if (gVertexGridId == duplicateGrid)
								duplicateFound = true;
						}
						if (!duplicateFound)
							mergedGrids.insert(gVertexGridId);

					}
					mergedGrids.insert(gridId);
					iter->second = mergedGrids;

				}
				
			}
		}	
	}
}

void RangeReachVertex::createRVertex(int MAX_REACH_GRIDS, LocationMap* spatialGraph){
	cout << "Create R Vertex" << endl;
	vector<int> GVerticesToErase;
	for (unordered_map<int,unordered_set<int>>::iterator iter = G_Vertex.begin(); iter != G_Vertex.end(); iter++) {
		int node = iter->first;
		if (iter->second.size() > MAX_REACH_GRIDS){
			R_Vertex[node] = MBR();
			R_Vertex[node] = MBRInsideGVertex[node];
			// for (Location pos : this->Locations_Inside_G_Vertex[node]) {
			// 	R_Vertex[node].insertLoc(pos);
			// }
			GVerticesToErase.push_back(node);
		}
	}
	for (int keyToErase: GVerticesToErase)
		G_Vertex.erase(keyToErase);
}

void RangeReachVertex::createBVertex(float MAX_RMBR, float totalSize){
	cout << "Create B Vertex" << endl;

	vector<int> RVerticesToDelete;

	for (unordered_map<int, MBR>::iterator it = R_Vertex.begin(); it != R_Vertex.end(); it++) 
	{
		float rVertexArea = fabs(it->second.xMax - it->second.xMin) * (it->second.yMax - it->second.yMin);
		if (totalSize * MAX_RMBR <= rVertexArea){
			this->B_Vertex[it->first] = true;
			RVerticesToDelete.push_back(it->first);
		}
	}

	for (int node : RVerticesToDelete)
		R_Vertex.erase(node);
}

void RangeReachVertex::checkVertexCorrectnes(){
	unordered_set<int> NodesInSet;
	vector<int> NodesInGVertex;
	vector<int> NodesInRVertex;
	vector<int> NodesInBVertex;

	for(unordered_map<int,unordered_set<int>>::iterator g_iter = G_Vertex.begin(); g_iter != G_Vertex.end(); g_iter++)
	{
		NodesInSet.insert(g_iter->first);
		NodesInGVertex.push_back(g_iter->first);
	}

	for(unordered_map<int, MBR>::iterator r_iter = R_Vertex.begin(); r_iter != R_Vertex.end(); r_iter++)
	{
		NodesInSet.insert(r_iter->first);
		NodesInRVertex.push_back(r_iter->first);
	}

	for(unordered_map<int, bool>::iterator b_iter = B_Vertex.begin(); b_iter != B_Vertex.end(); b_iter++)
	{
		NodesInSet.insert(b_iter->first);
		NodesInBVertex.push_back(b_iter->first);
	}
	cout << "Total Unique Nodes: " << NodesInSet.size();
	cout << "\nB Nodes: " << NodesInBVertex.size() << "\nG Nodes:" << NodesInGVertex.size() << "\nR Nodes: " << NodesInRVertex.size() << endl;
	int totalNodesInVertices = NodesInBVertex.size() + NodesInGVertex.size() + NodesInRVertex.size();
	cout << NodesInSet.size() << "  " << totalNodesInVertices << endl;;
}

bool RangeReachVertex::SpaReachQuery(int sourceNode, box queryWindow, Graph* socialGraph, LocationMap* spatialGraph, int layers) {
	
	MBR queryWindowToMBR(
		queryWindow.min_corner().get<0>(),
		queryWindow.min_corner().get<1>(),
		queryWindow.max_corner().get<0>(),
		queryWindow.max_corner().get<1>()
	);

	int node = sourceNode;
	if (socialGraph->NodeBelongsToSCC.count(node) != 0){
		node = socialGraph->NodeBelongsToSCC[node];
	}
	queue<int> Q;
	Q.push(node);
	while(!Q.empty()){

		int curr_node = Q.front();
		Q.pop();
		if (curr_node != node || socialGraph->SuperConnectedComponents.count(curr_node) != 0){
			if(spatialGraph->existLocation(curr_node)){
				vector<coordinates> pointsInsideScc = spatialGraph->getLocation(curr_node).spatialData;
				if (socialGraph->SuperConnectedComponents.count(curr_node)){
					for (int i = 4; i < pointsInsideScc.size() - 1; i += 2){

						Location pos = Location(pointsInsideScc[i], pointsInsideScc[i+1]);
						if (queryWindowToMBR.containsPoint(pos)){
							return true;
						}
					}
				} else {
					Location pos = Location(pointsInsideScc[0], pointsInsideScc[1]);
					if (queryWindowToMBR.containsPoint(pos)){
							return true;
					}
				}
			}
		}

		if (B_Vertex.count(curr_node) == 1 && B_Vertex[curr_node] == false){
			return false;
		}
		if (R_Vertex.count(curr_node) != 0){
			if (overlaps(R_Vertex[curr_node], queryWindow)){
				return true;
			}
			if (intersects(R_Vertex[curr_node], queryWindow)){
				for (int reachable_node : socialGraph->GraphScheme[curr_node]){
					Q.push(reachable_node);
				}
			} 
		}
		if (G_Vertex.count(curr_node) != 0){
			for (int gVertex : G_Vertex[curr_node]){
				MBR grid = this->getGridFieldById(gVertex);
				if (overlaps(grid, queryWindow)){
					return true;
				}
				if (intersects(grid, queryWindow)){
					for (int reachable_node : socialGraph->GraphScheme[curr_node]){
						Q.push(reachable_node);
					}
				} 
			}
		}
	}
	return false;
}

/*
Returns the spatial gridfield this for the location given in the parameters
*/
int RangeReachVertex::getSpatialGridField(Location nodeLocation, int layers) {
	bool spatialGridFound = false;
	int chunkIteratorX, chunkIteratorY;

	for(map<int, MBR>::iterator iter = this->gridLayers[0].begin(); iter != this->gridLayers[0].end(); iter++){
		if (iter->second.containsPoint(nodeLocation))
			return iter->first;
	}
	 
	nodeLocation.print();
	return -1;

	// for (int chunkIteratorX = 0; chunkIteratorX < layers; chunkIteratorX++) {
	// 	for (int chunkIteratorY = 0; chunkIteratorY < layers; chunkIteratorY++) {
	// 		if (nodeLocation.x >= maximumMBR.xMin + (chunkIteratorX)*chunkStepX && nodeLocation.x <= maximumMBR.xMin + (chunkIteratorX + 1) * chunkStepX) {
	// 			if (nodeLocation.y >= maximumMBR.yMin + (chunkIteratorY)*chunkStepY && nodeLocation.y <= maximumMBR.yMin + (chunkIteratorY + 1) * chunkStepY) {
	// 				return (chunkIteratorX + chunkIteratorY  * layers);
	// 			}
	// 		}
	// 	}
	// }
	// return -1;
}


int doesRectanglesIntersect(MBR queryArea,  MBR grid)
{
	box rec1 = box(point(queryArea.xMin, queryArea.yMin), point(queryArea.xMax, queryArea.yMax));
	box rec2 = box(point(grid.xMin, grid.yMin), point(grid.xMax, grid.yMax));

	bool doIntersect = boost::geometry::intersects(rec1, rec2);
	return doIntersect;
}


void RangeReachVertex::getAllSpatialGridsInArea(MBR queryArea, int LAYERS, vector<int>* reachableGrids) 
{
	coordinates gridXMin, gridYMin, gridXMax, gridYMax;
	int layers = LAYERS;

	this->chunkStepX = abs(maximumMBR.xMax - maximumMBR.xMin) / layers;
	this->chunkStepY = abs(maximumMBR.yMax - maximumMBR.yMin) / layers;

	for (int chunkIteratorX = 0; chunkIteratorX < layers; chunkIteratorX++)
	{
		for (int chunkIteratorY = 0; chunkIteratorY < layers; chunkIteratorY++)
		{
			MBR grid(
				maximumMBR.xMin + (chunkIteratorX)*chunkStepX,
				maximumMBR.yMin + (chunkIteratorY)*chunkStepY,
				maximumMBR.xMin + (chunkIteratorX + 1) * chunkStepX,
				maximumMBR.yMin + (chunkIteratorY + 1) * chunkStepY
			);


			if (doesRectanglesIntersect(queryArea, grid))
			{
				reachableGrids->push_back(chunkIteratorY + chunkIteratorX * layers);
			}
		}
	}
}


void RangeReachVertex::writeAttributesToFile(string filename){
	ofstream bFile, gFile, rFile, gridFile;
	bFile.open("./data/spareach/" + filename + "_b_vertex");
	for (unordered_map<int, bool>::iterator iter = B_Vertex.begin(); iter != B_Vertex.end(); iter++) {
		bFile << iter->first << "\t" << iter->second << "\n";
	}
	bFile.close();

	gFile.open("./data/spareach/" + filename + "_g_vertex");
	for (unordered_map<int,unordered_set<int>>::iterator iter = G_Vertex.begin(); iter != G_Vertex.end(); iter++) {
		gFile << iter->first;
		for (int node : iter->second) {
			gFile << "\t" << node;
		}
		gFile << "\n";
	}
	gFile.close();
	
	rFile.open("./data/spareach/" + filename + "_r_vertex");
	for (unordered_map<int, MBR>::iterator iter = R_Vertex.begin(); iter != R_Vertex.end(); iter++) {
		rFile << iter->first << "\t" << iter->second.stringify() << '\n';
	}
	rFile.close();


	gridFile.open("./data/spareach/" + filename + "_grid");
	for (map<int,map<int, MBR>>::iterator layerIterator = this->gridLayers.begin(); layerIterator != this->gridLayers.end(); layerIterator++){
		for (map<int, MBR>::iterator iter = this->gridLayers[layerIterator->first].begin(); iter != this->gridLayers[layerIterator->first].end(); iter++){
			gridFile << layerIterator->first << "\t" << iter->first << "\t" << iter->second.stringify()  << "\n";
		}	
	}
	gridFile.close();

}

void RangeReachVertex::readAttributesFromFile(string filename){
	cout << "Load attributes file" << endl;
    string bFilePath = "./data/spareach/" + filename + "_b_vertex";
    ifstream file;
    file.open(bFilePath);
    int node, bVertex;
	if (file.is_open()){
		while (file >> node >> bVertex) {
			this->B_Vertex[node] = bVertex == 1 ? true : false;
		}
	} else {
		cout << "B-Vertex File not found" << endl;
		return;
	}

    file.close();

    string gFilePath = "./data/spareach/" + filename + "_g_vertex";
	file.open(gFilePath);
	if (!file.is_open())
	{
		cout << "G-Vertex File not found" << endl;
		return;
	}
	string line;
	vector<int> gVertices;
	while (getline(file, line )) 
	{
		stringstream   linestream(line);
		string  data;
		while (getline(linestream, data, '\t')) 
		{
			gVertices.push_back(stoi(data));
		}
		int node = gVertices[0];
		gVertices.erase(gVertices.begin());
		this->G_Vertex[node] = unordered_set<int> (gVertices.begin(), gVertices.end());
		gVertices.clear();
	}
    file.close();



 	string rFilePath = "./data/spareach/" + filename + "_r_vertex";
    file.open(rFilePath);
	if (!file.is_open()){
		cout << "R-Vertex File not found" << endl;
		return;
	}

    coordinates xMin, xMax, yMin, yMax;
	while (file >> node >> xMin >> yMin >> xMax >> yMax) {
		this->R_Vertex[node] = MBR(xMin, yMin, xMax, yMax);
	}

	file.close();



	string gridfile = "./data/spareach/" + filename + "_grid";
    file.open(gridfile);
	if (!file.is_open()){
		cout << "Grid File not found" << endl;
		return;
	} else {
		int layer, gridId;
		coordinates xMin, xMax, yMin, yMax;
		while (file >> layer >> gridId >> xMin >> yMin >> xMax >> yMax) {
			this->gridLayers[layer][gridId] = MBR(xMin, yMin, xMax, yMax); 
		}
	}

	
	file.close();
}








#pragma region Print Statements

void RangeReachVertex::printAllGridLayers(){
	for (map<int,map<int, MBR>>::iterator layerIterator = this->gridLayers.begin(); layerIterator != this->gridLayers.end(); layerIterator++){
		cout << "Layer" << layerIterator->first << ": ";
		for (map<int, MBR>::iterator iter = this->gridLayers[layerIterator->first].begin(); iter != this->gridLayers[layerIterator->first].end(); iter++){
			cout << iter->first << "\t";
			iter->second.print();
			cout << endl;
		}	
	}

	for (map<int,map<int, MBR>>::iterator layerIterator = this->gridLayers.begin(); layerIterator != this->gridLayers.end(); layerIterator++){
		int gridCounter = 0;
		for (map<int, MBR>::iterator iter = this->gridLayers[layerIterator->first].begin(); iter != this->gridLayers[layerIterator->first].end(); iter++){
			gridCounter++;
		}	
		cout << "Layer " << layerIterator->first << " contains " << gridCounter << "grids \n ";
	}
	
}

void RangeReachVertex::printGridField(){
	cout << "Spatial Grid Cell:" << endl;
	for (map<int, MBR>::iterator iter = spatialGrid.begin(); iter != spatialGrid.end(); iter++){
		cout << iter->first << "\t";
		iter->second.print();
		cout << endl;
	}	
}

void RangeReachVertex::printBVertex() {
	cout << "B Vertex:\n";
	for (unordered_map<int, bool>::iterator it = B_Vertex.begin(); it != B_Vertex.end(); it++) {
		cout <<"N: "<< it->first << "\t" << it->second << endl;
	}
	cout << endl;
}

void RangeReachVertex::printRVertex() {
	cout << "R Vertex: \n";
	for (unordered_map<int, MBR>::iterator it = R_Vertex.begin(); it != R_Vertex.end(); it++) {
		cout << it->first << " -> " << it->second.getString() << endl;
	}
	cout << "\n" ;
}

void RangeReachVertex::printGVertex() {
	cout << "G Vertex: " << endl;
	for (unordered_map<int,unordered_set<int>>::iterator iter = G_Vertex.begin(); iter != G_Vertex.end(); iter++) {
		cout << iter->first << " : ";
		for (int grid : iter->second) {
			cout << grid << " ";
		}
		cout << endl;
	}
	cout << endl;
}



#pragma endregion



