#include "LocationMap.h"


LocationMap::LocationMap() {
    }

void LocationMap::addMinMaxCorners(vector<coordinates> corners) 
{
    this->MinMaxCorners = corners;
}

vector<coordinates> LocationMap::getMinMaxCorners() {
    return this->MinMaxCorners;
}


void LocationMap::addSpatialToMap(int vertex, spatialMbrRelation locationData) {
    Map[vertex] = locationData;
}
void LocationMap::addSpatial(int vertex, Location loc) {
    LocationScheme[vertex] = loc;
}

bool LocationMap::existLocation(int node) {
    return (Map.count(node) > 0);
}

spatialMbrRelation LocationMap::getLocation(int node) {
    return Map[node];
}

Location LocationMap::getVanillaLocation(int node) {
    return LocationScheme[node];
}

unordered_map <int, Location> LocationMap::returnVanillaMap() {
    return LocationScheme;
}

unordered_map <int, spatialMbrRelation> LocationMap::returnMap() {
    return Map;
}


void LocationMap::createMbrs(unordered_map<int, vector<int>>* Scc) {
    cout << "Create Mbrs for strongly connected components" << endl;
    int supernode;
    
    vector<coordinates> mbrData;
    coordinates newXpos, newYpos;
    bool spatialExist = false;
    mbrData.push_back(INT_MAX);
    mbrData.push_back(INT_MAX);
    mbrData.push_back(INT_MIN);
    mbrData.push_back(INT_MIN);
    for(unordered_map<int, vector<int>>::iterator iter = Scc->begin(); iter != Scc->end(); iter++)
    {
        supernode = iter->first;
        for (int currnode : iter->second)
        {
            if (this->existLocation(currnode)) {
                spatialExist = true;
                spatialMbrRelation spatialData = this->getLocation(currnode);
                newXpos = spatialData.spatialData[0];
                newYpos = spatialData.spatialData[1];
                if (newXpos < mbrData[0]) { mbrData[0] = newXpos; }
                if (newYpos < mbrData[1]) { mbrData[1] = newYpos; }
                if (newXpos > mbrData[2]) { mbrData[2] = newXpos; }
                if (newYpos > mbrData[3]) { mbrData[3] = newYpos; }
                mbrData.push_back(newXpos);
                mbrData.push_back(newYpos);
            }
        }
        if(spatialExist)
            this->Map[supernode] = spatialMbrRelation(true, mbrData);
        spatialExist = false;
        mbrData.clear();
        mbrData.push_back(INT_MAX);
        mbrData.push_back(INT_MAX);
        mbrData.push_back(INT_MIN);
        mbrData.push_back(INT_MIN);
    }


};

void LocationMap::writeMapToFile(string filePath)
{
    unordered_map<int, spatialMbrRelation>::iterator it;
    ofstream file(filePath);
    for (it = this->Map.begin(); it != this->Map.end(); it++)
    {
        file << it->first << "\t";
        for (coordinates node : this->Map[it->first].spatialData) {
            file << node << "\t";
        }
        file << "\n";
    } 
}

void LocationMap::readFileForMap(string filePath)
{
    cout << "Read reduced spatial data" << endl;
    
    vector<coordinates> _minMaxCorners;
    _minMaxCorners.push_back(INT_MAX);
    _minMaxCorners.push_back(INT_MAX);
    _minMaxCorners.push_back(INT_MIN);
    _minMaxCorners.push_back(INT_MIN);


    ifstream file;
    file.open(filePath);
    string line;
    vector<coordinates> SpatialData;
    int node;
    int j = 0;
    if (file.is_open()) {
        while (getline(file, line))
        {
            j++;
            stringstream linestream(line);
            string data;
            while (getline(linestream, data, '\t')) {
                SpatialData.push_back(stof(data));
            }
            node = SpatialData[0];
            SpatialData.erase(SpatialData.begin());

            bool isMbr;
            if (SpatialData[0] != SpatialData[2] || SpatialData[1] != SpatialData[3])
            {
                isMbr = true;
            }else
            {
                isMbr = false;
            }
            this->Map[node] = spatialMbrRelation(isMbr, SpatialData);

            
            if (SpatialData[1] < _minMaxCorners[0]) _minMaxCorners[0] = SpatialData[1];
            if (SpatialData[2] < _minMaxCorners[1]) _minMaxCorners[1] = SpatialData[2];
            if (SpatialData[3] > _minMaxCorners[2]) _minMaxCorners[2] = SpatialData[3];
            if (SpatialData[4] > _minMaxCorners[3]) _minMaxCorners[3] = SpatialData[4];        
            SpatialData.clear();
        }
    }
    this->MinMaxCorners = _minMaxCorners;
}

void LocationMap::printLocations() {
    cout << "Location Map | Size -> " << LocationScheme.size() << endl;
    for (auto i : LocationScheme) {
        cout << "Node: " << i.first << " |  Position ( " << i.second.x << " , " << i.second.y << " )" << endl;
    }
    cout << endl;
}

void LocationMap::printMap() {
    cout << "SpatialData : \n";
    unordered_map<int, spatialMbrRelation>::iterator it;
    for (it = Map.begin(); it != Map.end(); it++) {
        if (it->second.isMbr){
            cout <<"MBR " << it->first << ": ";
        } else {
            cout <<"PNT " << it->first << ": ";
        }
        for (coordinates i : it->second.spatialData)
        {
            cout << i << " ";
        }
        cout << endl;
    }
}
