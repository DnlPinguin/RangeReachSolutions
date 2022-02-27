#include "main.h"

vector<float> readSpatialData(string fileName, rTreePlanes* rTree) {
    ifstream file;
    file.open("./data/raw/" + fileName + "_spatial");
    int node;
    cout << "Read Spatial data" << endl;
    coordinates xMin = INT_MAX, xMax = INT_MIN, yMin = INT_MAX, yMax = INT_MIN;
    coordinates xPos, yPos;
    string line;
    int counter = 0;
    int totalCounter = 0;
    if (file.is_open()) {
        while (getline(file, line)){
            stringstream ss(line);
            string _node, _xpos, _ypos;
            getline(ss,_node,',');    
            getline(ss,_xpos,','); 
            getline(ss,_ypos,','); 
            node = stoi(_node);
            xPos = stof(_xpos);
            yPos = stof(_ypos);
            rTree->insert(make_pair(point(xPos,yPos), node));
            if (xPos < xMin) { xMin = xPos; }
            if (yPos < yMin) { yMin = yPos; }
            if (xPos > xMax) { xMax = xPos; }
            if (yPos > yMax) { yMax = yPos; }
            totalCounter++;
        }
    }else{

    }
    cout << totalCounter << " points read. \n";
    file.close();
    vector<float> minMaxCorner = {xMin, yMin, xMax, yMax};
    return minMaxCorner;

}

vector<vector<float>> createQueryBoxes(vector<float> areaSizeVector, vector<float> minMaxCorner, int boxMovements){
    cout << "Create Query Boxes" << endl;
    vector<vector<float>> areaBoxes; 

    for ( float areaSize : areaSizeVector){
        float xLength = minMaxCorner[2] - minMaxCorner[0];
        float yLength = minMaxCorner[3] - minMaxCorner[1];
        vector<float>xLinspace=  createLinspace(minMaxCorner[0], minMaxCorner[2], boxMovements);
        vector<float>yLinspace=  createLinspace(minMaxCorner[1], minMaxCorner[3], boxMovements);

        for (int i = 0; i < boxMovements; i++){
            float stepX = xLength * areaSize;
            float stepY = yLength * areaSize;

            if(xLinspace[i] + stepX <= minMaxCorner[2] && yLinspace[i] + stepY <= minMaxCorner[3])
            {
                vector<float> box {
                    xLinspace[i], 
                    yLinspace[i],
                    xLinspace[i] + yLength, 
                    yLinspace[i] + stepY,
                    areaSize
                };
                areaBoxes.push_back(box);
            }
        }
    }
    return areaBoxes;
}

map<int,int> getQueryNodes(Graph* SocialGeoGraph, int amountOfQueryNodes){
    cout << "Get Query nodes " << endl;
    map<int,int> node_degree_map;
    map<int,int > queryNodes;

    for (unordered_map<int, vector<int>>::iterator node = SocialGeoGraph->GraphScheme.begin(); node != SocialGeoGraph->GraphScheme.end(); node++) {
        node_degree_map[node->first] = node->second.size();
    }

    if (node_degree_map.size() < amountOfQueryNodes){
        return node_degree_map;
    }

    int sumOfAllDegrees = 0;


    int counter = 0; 
    for (auto it = node_degree_map.rbegin(); it != node_degree_map.rend(); it++) {
        sumOfAllDegrees += it->second; 
    }
    int averageOfAllDegrees = sumOfAllDegrees / node_degree_map.size();


    for (auto iter = node_degree_map.rbegin(); iter != node_degree_map.rend(); ++iter){
        if (averageOfAllDegrees + 5  < iter->second){
            queryNodes[iter->first] = iter->second;
        }
    }

    for(auto iter = node_degree_map.rbegin(); iter != node_degree_map.rend(); ++iter){
        if (queryNodes.size() < amountOfQueryNodes){
            queryNodes[iter->first] = iter->second;
        }
    }
    return queryNodes;
}


int main(int argc, char **argv){
    if (argc < 3){
		cout << "Arguments missing! Try createQueries {{fileName}} {{numberOfQueries}}" << endl;
		return 0;
	}

    string filename = argv[1];
    int numberOfQueries = stoi(argv[2]);


    Graph SocialGeoGraph;
    SocialGeoGraph.readReducedGraph("./data/processed/" + filename + "_reduced_scheme");
    
    rTreePlanes rtree ;
    vector<float> areaSize {1, 0.5, 0.1, 0.05, 0.01};
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