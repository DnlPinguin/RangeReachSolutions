#include "main.h"

vector<float> readSpatialData(string fileName, rTreePlanes* rTree) {
    ifstream file;
    file.open("./data/raw/" + fileName + "_spatial");
    int node;
    coordinates xMin = INT_MAX, xMax = INT_MIN, yMin = INT_MAX, yMax = INT_MIN;
    coordinates xPos, yPos;
    string time;

    if (file.is_open()) {
        while (file >> node >>  xPos >> yPos) {
            rTree->insert(make_pair(point(xPos,yPos), node));
            if (xPos < xMin) { xMin = xPos; }
            if (yPos < yMin) { yMin = yPos; }
            if (xPos > xMax) { xMax = xPos; }
            if (yPos > yMax) { yMax = yPos; }
        }
    }else{
    }
    file.close();
    vector<float> minMaxCorner = {xMin, yMin, xMax, yMax};
    return minMaxCorner;

}

vector<vector<float>> createQueryBoxes(vector<float> areaSizeVector, vector<float> minMaxCorner, int boxMovements){
    float maxAreaSize = *max_element(std::begin(areaSizeVector), std::end(areaSizeVector));
    float xMaximumLength =  (minMaxCorner[2]- minMaxCorner[0]) * maxAreaSize;
    float yMaximumLength = (minMaxCorner[3]- minMaxCorner[1]) * maxAreaSize;
    vector<float> maximumCorner {minMaxCorner[2] - xMaximumLength, minMaxCorner[3]- yMaximumLength};
    vector<float>xLinspace=  createLinspace(minMaxCorner[0], minMaxCorner[2]- xMaximumLength, boxMovements);
    vector<float>yLinspace=  createLinspace(minMaxCorner[1], minMaxCorner[3]- yMaximumLength, boxMovements);


    vector<vector<float>> areaBoxes;
    float xLength, yLength;

    for (float areaSize : areaSizeVector){
        xLength = (minMaxCorner[2]- minMaxCorner[0]) * areaSize;
        yLength = (minMaxCorner[3]- minMaxCorner[1]) * areaSize;

        for (int i = 0; i < boxMovements; i++){
            vector<float> box {
                xLinspace[i], 
                yLinspace[i],
                xLinspace[i] + xLength, 
                yLinspace[i] + yLength,
                areaSize
            };
            areaBoxes.push_back(box);
        }
    }

    return areaBoxes;
}

map<int,int> getQueryNodes(Graph* SocialGeoGraph, int amountOfQueryNodes){
    map<int,int> node_degree_map;
    map<int,int > queryNodes;

    for (unordered_map<int, vector<int>>::iterator node = SocialGeoGraph->GraphScheme.begin(); node != SocialGeoGraph->GraphScheme.end(); node++) {
        node_degree_map[node->first] = node->second.size();
    }

    if (node_degree_map.size() < amountOfQueryNodes){
        return node_degree_map;
    }

    int sumOfAllDegrees = 0;
    for (map<int, int>::iterator it = node_degree_map.begin(); it != node_degree_map.end(); it++) {
        sumOfAllDegrees += it->second; 
    }
    int averageOfAllDegrees = sumOfAllDegrees / node_degree_map.size();

    int step= 0;
    while (averageOfAllDegrees > node_degree_map[step]){
        step++;
    }

    for (int i = 0; queryNodes.size() <= 50; i++){
        try {
            queryNodes[node_degree_map.at(step + i)] = node_degree_map.at(step + i);
        }
        catch (const std::exception &e) {
            i++;
            queryNodes[node_degree_map.at(step - i)] = node_degree_map.at(step - i);
        }
        try {
            queryNodes[node_degree_map.at(step - i)] = node_degree_map.at(step - i);
        }
        catch (const std::exception &e) {
            i++;
            queryNodes[node_degree_map.at(step + i)] = node_degree_map.at(step + i);
        }
    }
    return queryNodes;
    

}


int main(int argc, char **argv){
    if (argc < 3){
		cout << "Arguments missing! Try createQueries {{fileName}} {{numberOfQueries}} {{degree||area}}" << endl;
		return 0;
	}

    string filename = argv[1];
    int numberOfQueries = stoi(argv[2]);
    string queryType = argv[3];


    Graph SocialGeoGraph;
    SocialGeoGraph.readReducedGraph("./data/processed/" + filename + "_reduced_scheme");
    
    rTreePlanes rtree ;
    vector<float> areaSize {0.5,0.005, 0.001, 0.0005, 0.0001};
    vector<float> minMaxCorner = readSpatialData(filename, &rtree);
    vector<vector<float>> areaBoxes = createQueryBoxes(areaSize, minMaxCorner, 100);
    
    map<int,int> queryNodes = getQueryNodes(&SocialGeoGraph, numberOfQueries);

    int nodesInRegion;
    ofstream file;
    
    file.open("./data/queries/" + filename +"_queries");

    file << "node\tdegree\txLow\tyLow\txTop\tyLow\tarea\tarea" << endl;
    for (vector<float> i : areaBoxes){
        nodesInRegion = computeCardinality(&rtree, box(point(i[0], i[1]), point(i[2], i[3])));
        for (map<int,int>::iterator iter = queryNodes.begin(); iter != queryNodes.end(); iter++){
            file << iter->first << "\t" << iter->second <<  "\t"<<  i[0] << "\t" << i[1] << "\t" << i[2] << "\t" << i[3] << "\t" << i[4] << "\t" << nodesInRegion << endl;
        }
    }
    file.close();
    return 0;
}