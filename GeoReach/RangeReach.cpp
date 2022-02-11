#include "RangeReach.h"


/*
Checks whether the sspatial grid is intersecting with the spatial region. If not we can say the node is not reaching any nodes lying the the spatial target region
*/
bool intersects(MBR spatialGrid, box queryWindow) {
	cout << "Check intersection: " << endl;
	coordinates queryWindowMinX = queryWindow.min_corner().get<0>();
	coordinates queryWindowMinY = queryWindow.min_corner().get<1>();
	coordinates queryWindowMaxX = queryWindow.max_corner().get<0>();
	coordinates queryWindowMaxY = queryWindow.max_corner().get<1>();

 	if (spatialGrid.xMax < queryWindowMinX || spatialGrid.xMin > queryWindowMaxX) {
		cout << "spatialgrid lies left or right" << endl;
		return false;
	}
	// If one rectangle is above other 
	if (spatialGrid.yMax < queryWindowMinY || spatialGrid.yMin > queryWindowMaxY) {
		cout << "spatialgrid lies top or bottom" << endl;
		return false;
	}

	return true;
}

/*
 Checks if the spatial region is overlapping the spatial grid returned by the vertex. If so, the whole grid lies inside the spatial region and the node is automatically
in this region too. 
*/
bool overlaps(MBR spatialGrid, box queryWindow) {
	if (true) 
	{
		cout << "Spatialgrid: {(" << spatialGrid.xMin << ", " << spatialGrid.yMin << "),(" << spatialGrid.xMax <<", " << spatialGrid.yMax << ")}" << endl;
		cout << "Querywindow: {(" <<
			queryWindow.min_corner().get<0>() << ", " <<
			queryWindow.min_corner().get<1>() << "),(" <<
			queryWindow.max_corner().get<0>() << ", " <<
			queryWindow.max_corner().get<1>()
			<< ")} \n";
	}
	if (spatialGrid.xMin >= queryWindow.min_corner().get<0>()
		&& spatialGrid.yMin >= queryWindow.min_corner().get<1>()
		&& spatialGrid.xMax <= queryWindow.max_corner().get<0>()
		&& spatialGrid.yMax <= queryWindow.max_corner().get<1>()
	)
	{
		cout << "Spatialgrid is overlapping queryWindow \n \n";;
		return true;
	}
	cout << "Spatialgrid is not overlapping queryWindow \n \n" ;
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

void RangeReachVertex::CreateRangeReachVertex(Graph* socialGraph, LocationMap* locationGraph, float MAX_RMBR = 1, float MAX_REACH_GRIDS = 3, int LAYERS=4) {
	cout << "create SPA-Graph structure  \n";

	vector<int> topologicalOrderedVertices;
	socialGraph->topologicalSort(&topologicalOrderedVertices);

	vector<coordinates> maxMBR = locationGraph->getMinMaxCorners();

	this->maximumMBR = MBR(maxMBR[0], maxMBR[1], maxMBR[2], maxMBR[3]);
	this->maximumMBR = MBR(0, 0, 1, 1);

	
	this->createGridField(LAYERS);

	/**
	 * Create Vertices by iterating trough the topological order in reverse
	 */
	for (int iter = topologicalOrderedVertices.size(); iter >= 0; iter--)
	{
		int currentNode = topologicalOrderedVertices[iter];
		

		for (int reachableNode : socialGraph->GraphScheme[currentNode])
		{

			// If a reachable node has a spatial attribute, we can add it to the vertex structure			
			if (locationGraph->existLocation(reachableNode))
			{
				//B_Vertex is set to true, since it is able to reach atleast one node, that as a spatial attribute
				B_Vertex[currentNode] = true;

				/**
				 * IMPORTANT: Check if the reachable node is a strongly connected component
				 * If so, we have to add the child element of the strongly connected component
				 * to the vertices.
				 * If not, we only add the node
				 */
				if (socialGraph->SuperConnectedComponents.count(reachableNode))
				{
					for (int nodeInScc : socialGraph->SuperConnectedComponents[reachableNode]){
						if (locationGraph->existLocation(nodeInScc)){
							Location locationOfNodeInScc = locationGraph->getVanillaLocation(nodeInScc);

							/**
							 * Aslong the G_Vertex of our current node doesnt exceed the maximum limit defined
							 * in MAX_REACH_GRIDS, we can append the spatial grid field of the reachable 
							 * node into the G_Vertex.
							 * Else we need to expand our R_Vertex
							 * 
							 */
							if (G_Vertex[currentNode].size() < MAX_REACH_GRIDS)
							{
								G_Vertex[currentNode].insert(getSpatialGridField(locationOfNodeInScc, LAYERS));
								Nodes_In_G_Vertex[currentNode].push_back(nodeInScc);
							} 
							else
							{
								/**
								 * If there was no R_Vertex established, we first need to create a new one,
								 * by creating an minimum bounding rectangle consisting of all nodes
								 * that are currently stored inside the G_Vertex;
								 * 
								 */
								if (this->R_Vertex.count(currentNode) == 0)
								{
									this->R_Vertex[currentNode] = MBR(locationOfNodeInScc);
									for (int node : Nodes_In_G_Vertex[currentNode]) {
										this->R_Vertex[currentNode].insertLoc(locationGraph->getVanillaLocation(node));
									}
								}
								/**
								 * After that, we just insert the new found location
								 * 
								 */
								this->R_Vertex[currentNode].insertLoc(locationOfNodeInScc);
							}			
						}
								
					}
				} 
				else
				{
					/**
					 * Same process for non strongly connected components
					 */
					if (G_Vertex[currentNode].size() < MAX_REACH_GRIDS)
					{
						G_Vertex[currentNode].insert(getSpatialGridField(locationGraph->getVanillaLocation(reachableNode), LAYERS));
						Nodes_In_G_Vertex[currentNode].push_back(reachableNode);
					} 
					else
					{
						if (this->R_Vertex.count(currentNode) == 0)
						{
							this->R_Vertex[currentNode] = MBR(locationGraph->getVanillaLocation(reachableNode));
							for (int node : Nodes_In_G_Vertex[currentNode]) {
								this->R_Vertex[currentNode].insertLoc(locationGraph->getVanillaLocation(node));
							}
						}
						this->R_Vertex[currentNode].insertLoc(locationGraph->getVanillaLocation(reachableNode));
					}
				}
			} 
			/**
			 * Additionally, there is still the possibility, that the reachable node
			 * itself has no spatiala attribute, but its vertices were already enhanced.
			 * In this case, we check if B_Vertex is true, and insert the already existing
			 * vertices into our current node
			 */

			if (B_Vertex[reachableNode])
			{

				B_Vertex[currentNode] = true;
				/** 
				 * If a R-Vertex already exists in reachable node, we can append it to the current node
				*/
				if (R_Vertex.count(reachableNode) > 0)
				{

					if (R_Vertex.count(currentNode))
					{
						R_Vertex[currentNode].insertMBR(R_Vertex[reachableNode]);
					} else
					{
						R_Vertex[currentNode] = MBR(R_Vertex[reachableNode]);
					}
					if (locationGraph->existLocation(reachableNode)){
						this->R_Vertex[currentNode].insertLoc(locationGraph->getVanillaLocation(reachableNode));
					}
				} 
				else 
				{
					/**
					 * Otherwise we have to pick every node from the g vertex and incrementally add it to the 
					 * current node
 					 */

					for (int gVertex : G_Vertex[reachableNode])
					{

						if (G_Vertex[currentNode].size() < MAX_REACH_GRIDS)
						{
							G_Vertex[currentNode].insert(gVertex);
							
							for (int nodeInGVertex : Nodes_In_G_Vertex[reachableNode])
							{
								if (!(find(Nodes_In_G_Vertex[currentNode].begin(), Nodes_In_G_Vertex[currentNode].end(), nodeInGVertex) != Nodes_In_G_Vertex[currentNode].end()))
									Nodes_In_G_Vertex[currentNode].push_back(nodeInGVertex);
							}

						}
						else
						{
							if (this->R_Vertex.count(currentNode) == 0)
							{
								this->R_Vertex[currentNode] = MBR(locationGraph->getVanillaLocation(Nodes_In_G_Vertex[currentNode][0]));

								for (int node : Nodes_In_G_Vertex[currentNode]) {
									this->R_Vertex[currentNode].insertLoc(locationGraph->getVanillaLocation(node));
								}
							}
							if (locationGraph->existLocation(reachableNode))
							{
								if (socialGraph->SuperConnectedComponents.count(reachableNode)){
									for (int nodeInScc : socialGraph->SuperConnectedComponents[reachableNode])
									{
										if (locationGraph->existLocation(nodeInScc))
										{
											this->R_Vertex[currentNode].insertLoc(locationGraph->getVanillaLocation(nodeInScc));
										}
									}
								} 
								else 
								{
									this->R_Vertex[currentNode].insertLoc(locationGraph->getVanillaLocation(reachableNode));
								}
							}
						}
					}
				}
			}
				
		}
		if (B_Vertex.count(currentNode) == 0) {
			B_Vertex[currentNode] = false;
		}
	}


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

	// cout << "lowerlayer " << lowerLayer << endl;
	// cout << "HighGrid " << higherLevelGrid.stringify() << endl;

	for (gridIterator; gridIterator != this->gridLayers[lowerLayer].end(); gridIterator++){
		int gridId = gridIterator->first;
		MBR grid = gridIterator->second;
		// cout << "LowerGrid " << gridId << " : " << grid.stringify() << endl;
	
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
		// cout << "layer " << currentLayer  << " : " << this->gridLayers[currentLayer].size() << endl;	
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
				if (sameGrids > MERGE_COUNT){
					// cout << "g_vertex: ";
					// for (int gVertexGridId : iter->second)
					// 	cout << gVertexGridId << " ";
					// cout << endl << "lowerGrid: ";
					// for (int lowerGridId : lowerLevelGrids)
					// 	cout << lowerGridId << " ";
					// cout << endl << "same grids: ";
					// for (int i : sameGridIds)
						// cout << i << " ";

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

					// cout << endl << " => " << sameGrids << endl;
					// cout << "current grid" << gridId << endl;
				}
				
			}
		}	
	}
}

void RangeReachVertex::createRVertex(int MAX_REACH_GRIDS, LocationMap* spatialGraph){
	// for (unordered_map<int,unordered_set<int>>::iterator iter = G_Vertex.begin(); iter != G_Vertex.end(); iter++) {
	// 	int node = iter->first;
	// 	if (iter->second.size() > MAX_REACH_GRIDS){
	// 		for (int nodeWithSpatial : this->Nodes_In_G_Vertex[node]) {
				

	// 			if (this->R_Vertex.count(nodeWithSpatial) == 0)
	// 			{
	// 				this->R_Vertex[nodeWithSpatial] = MBR(spatialGraph->getVanillaLocation(nodeWithSpatial));
	// 			} else 
	// 			{
	// 				this->R_Vertex[nodeWithSpatial]
	// 			}
	// 		}
	// 	}
		
	// 	cout << endl;
	// }
	// cout << endl;
}


bool RangeReachVertex::SpaReachQuery(int node, box queryWindow, Graph* socialGraph, LocationMap* spatialGraph, int layers) {
	if (socialGraph->NodeBelongsToSCC.count(node))
	{
		node = socialGraph->NodeBelongsToSCC[node];
	}
	//printBox(queryWindow);
	
	MBR queryWindowToMBR(
		queryWindow.min_corner().get<0>(),
		queryWindow.min_corner().get<1>(),
		queryWindow.max_corner().get<0>(),
		queryWindow.max_corner().get<1>()
	);
	
	vector<int> reachableGrids;
	getAllSpatialGridsInArea(queryWindowToMBR, layers, &reachableGrids);
	
	cout << "spatialGrid in area: ";
	for (int i : reachableGrids){
		cout << i << " ";
	}
	cout << endl;
	
	queue<int> Q;
	Q.push(node);
	cout << "run for " << node << endl;
	while(!Q.empty())
	{
		int curr_node = Q.front();
        Q.pop();
		if (spatialGraph->existLocation(curr_node) && curr_node != node)
		{
			cout << curr_node << " exists location" << endl;
			Location loc = spatialGraph->getVanillaLocation(curr_node);
			//cout << queryWindow.min_corner().get<0>() << " " << loc.x << " "<<queryWindow.max_corner().get<0>() << endl; 
			if (queryWindow.min_corner().get<0>() <= loc.x && loc.x <= queryWindow.max_corner().get<0>())
			{
				//cout << " x is inside \n";
				if (queryWindow.min_corner().get<1>() <= loc.y && loc.y <= queryWindow.max_corner().get<1>())
				{
					//cout << " y is inside \n";
					return true;
				}
			}
		}
		if (B_Vertex[curr_node] == true) //R_Vertex doesnt exist
		{
			cout << curr_node << " b is true " << endl;
			//cout << curr_node << " B_Vertex is true. \n";
			if (R_Vertex.count(curr_node) == 0)
			{
				cout << curr_node << " has no b attribute" << endl;
				//cout << curr_node << " R_Vertex is false. " << this->G_Vertex[curr_node].size() << "\n";
				for (int i : this->G_Vertex[curr_node])
				{
		
					if (overlaps(getGridFieldById(i), queryWindow))
					{
							cout << "queryWindow overlaps G-Vertex. \n";
							return true;
					}
					if (intersects(getGridFieldById(i), queryWindow))
					{
							cout << "G_Vertex intersects query window. "<< i <<"\n";
							vector<int> reachable_nodes = socialGraph->GraphScheme[curr_node];
							for (int node : reachable_nodes)Q.push(node);
					}
				}
			}
			else // R_Vertex exists
			{
				cout << curr_node << " has r attribute" << endl;
				if (overlaps(R_Vertex[curr_node], queryWindow))
				{
					cout << "queryWindow overlaps R-Vertex. \n";
					return true;
				}
				if (intersects(R_Vertex[curr_node], queryWindow))
				{
					cout << curr_node << " R_Vertex interesects query window. \n";
					vector<int> reachable_nodes = socialGraph->GraphScheme[curr_node];
					for (int i : reachable_nodes)Q.push(i);
				} 
			}	
		}
	}

	cout << "Return False \n";
	cout << endl;
	return false;
}




/*
Returns the spatial gridfield this for the location given in the parameters
*/
int RangeReachVertex::getSpatialGridField(Location nodeLocation, int layers) {
	bool spatialGridFound = false;
	int chunkIteratorX, chunkIteratorY;

	// nodeLocation.print();
	for (int chunkIteratorX = 0; chunkIteratorX < layers; chunkIteratorX++) {
		for (int chunkIteratorY = 0; chunkIteratorY < layers; chunkIteratorY++) {
			if (nodeLocation.x >= maximumMBR.xMin + (chunkIteratorX)*chunkStepX && nodeLocation.x <= maximumMBR.xMin + (chunkIteratorX + 1) * chunkStepX) {
				if (nodeLocation.y >= maximumMBR.yMin + (chunkIteratorY)*chunkStepY && nodeLocation.y <= maximumMBR.yMin + (chunkIteratorY + 1) * chunkStepY) {
					// cout << chunkIteratorY + chunkIteratorX * layers << ": "; 
					// this->gridLayers[0][chunkIteratorY + chunkIteratorX * layers].print();
					return (chunkIteratorX + chunkIteratorY  * layers);
				}
			}
		}
	}
	return -1;
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

			// cout << "MBR: " << queryArea.stringify() << endl;
			// cout << "Grid: " << grid.stringify() << endl;

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
	cout << "B Vertex: \n";
	for (auto B : B_Vertex) {
		cout <<" Node:  "<< B.first << "\t" << B.second << endl;
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



