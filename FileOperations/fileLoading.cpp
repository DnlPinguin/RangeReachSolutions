#include "fileLoading.h"
#include <sstream>
#include <iostream>

int getMaximumNode(int one, int two, int max)
{
    if (one > max) { max = one; }
    if (two > max) { max = two; }
    return max;
}



void loadFile(string fileName, Graph* graphToBuild) {
    cout << "Load social file" << endl;
    string filePath ="./data/raw/" + fileName + "_social";
    ifstream socialFile;
    socialFile.open(filePath);
    int nodeOne, nodeTwo;
    int maxNode = INT_MIN;
    if (socialFile.is_open()) {
        while (socialFile >> nodeOne >> nodeTwo) {
            maxNode = getMaximumNode(nodeOne, nodeTwo, maxNode);
            graphToBuild->addEdge(nodeOne, nodeTwo);
        }
    }
    else {
        cout << "no file found" << endl;
    } 
    graphToBuild->addMaximumNode(maxNode);
    socialFile.close();
}

void loadVanillaGeoFileData(string fileName, LocationMap* locs) {
    cout << "Load spatial file" << endl;
    string filePath = "./data/raw/" + fileName + "_spatial";
    ifstream file;
    file.open(filePath);
    int node;
    int maxNode = INT_MIN;
    coordinates xMin = INT_MAX, xMax = INT_MIN, yMin = INT_MAX, yMax = INT_MIN;
    coordinates xPos, yPos;
    string time;

    if (file.is_open()) {
        while (file >> node >>  xPos >> yPos) {
            if (xPos < xMin) { xMin = xPos; }
            if (yPos < yMin) { yMin = yPos; }
            if (xPos > xMax) { xMax = xPos; }
            if (yPos > yMax) { yMax = yPos; }
            locs->addSpatial(node, Location(xPos, yPos));
            locs->addSpatialToMap(node, spatialMbrRelation(false, vector<coordinates>{xPos, yPos, xPos, yPos}));
        }
    }else{
        cout << "No file found" << endl;
    }
    locs->addMinMaxCorners(vector<coordinates>{ xMin, yMin, xMax, yMax });
    file.close();
}

void loadGeoFileData(string fileName, LocationMap* locs) {
    cout << "Load spatial file..." << endl;
    ifstream file;
    file.open(fileName);
    vector<coordinates> spatialData;
    coordinates xMin = INT_MAX, xMax = INT_MIN, yMin = INT_MAX, yMax = INT_MIN;
    bool isMbr;
    string line;
    int node;
    if (file.is_open())
    {
        while (getline(file, line )) 
        {
            stringstream   linestream(line);
            string  data;
            while (getline(linestream, data, '\t')) 
            {
                spatialData.push_back(stoi(data));
            }

            node = spatialData[0];
            spatialData.erase(spatialData.begin());
            if (spatialData[0] < xMin) { xMin = spatialData[0]; }
            if (spatialData[1] < yMin) { yMin = spatialData[1]; }
            if (spatialData[2] > xMax) { xMax = spatialData[2]; }
            if (spatialData[3] > yMax) { yMax = spatialData[3]; }

            if ((spatialData[1] == spatialData[3]) && (spatialData[2] == spatialData[4]))
            {
                //spatialData[0] = 0;
                isMbr = false;
            }
            else 
            {
                //spatialData[0] = 1;
                isMbr = true;
            }
            pair<int, vector<coordinates>> spatialObjet(isMbr, spatialData);

            locs->addSpatialToMap(node, spatialMbrRelation(isMbr, spatialData));
            spatialData.clear();
        }
    }
    file.close();
    locs->addMinMaxCorners(vector<coordinates>{ xMin, yMin, xMax, yMax });
    cout << "Spatial file fully loaded." << endl;
}

void loadSuperConnectedComponents(string fileName, Graph* graphToExpand, LocationMap* LocationGraph)
{
    cout << "Load Super Connected Components..." << endl;
    ifstream file;
    file.open(fileName);
    string line;
    bool isNode = false;
    int superNode;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            isNode = false;
            string delimiter = "\t";
            if (line.find(delimiter) != string::npos)
            {
                size_t pos = 0;
                string token;
                while ((pos = line.find(delimiter)) != std::string::npos) {
                    token = line.substr(0, pos);
                    if (token != "->")
                    {
                        if (isNode)
                        {
                            graphToExpand->addSCC(superNode, stoi(token));
                        }
                        else {
                            superNode = stoi(token);
                        }
                    }
                        else
                    {
                        isNode = true;
                    }
                    line.erase(0, pos + delimiter.length());
                }
            }
        }
    }
    file.close();

}

vector<SpatialNode>* readGeoFile(string filePath, LocationMap* spatialGraph)
{
    vector<SpatialNode>* data = new vector<SpatialNode>;
    ifstream queryFile;
    int node;
    coordinates x,y;

    cout << filePath << endl;
    queryFile.open(filePath);
    if (queryFile.is_open()) 
    {
        cout << "file is open \n";
        while (queryFile >> node >> x >> y) 
        {
            SpatialNode spatial = make_pair(point(x,y), node);
            data->push_back(spatial);
            spatialGraph->addSpatial(node, Location(x, y));
        }
    }
    cout << data->size() << " points loaded.\n";
    return data;
}

vector<queryParameter>* readQueries(string filePath)
{
    vector<queryParameter>* queries = new vector<queryParameter>;
    ifstream queryFile;
    int node, degree, cardinality;
    float area;
    coordinates xMin, yMin, xMax, yMax;

    cout << filePath << endl;
    queryFile.open(filePath);
    if (queryFile.is_open()) {
        cout << "file is open \n";
        while (queryFile >> node >> degree >> xMin >> yMin >> xMax >> yMax >> area >> cardinality) {
            box spatial = box(point(xMin, yMin), point(xMax, yMax));
            queries->push_back(
                queryParameter(node, degree, spatial, area, cardinality)
            );
        }
    }
    cout << queries->size() << " queries loaded.\n";
    return queries;
}


