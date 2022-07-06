#include "main.h"

int main(int argc, char **argv){
    Timer clock;
	Graph SocialGeoGraph;
	LocationMap LocationGraph;

    string filename;
    if (argc == 2){
        filename = argv[1];
    }  else {
        cout << "Enter filename: \n";
        cin >> filename; 
    }

    double completeConstructionTime, rTreeConstructionTime, sccConstructiontime, intervalConstructionTime;
    SocialGeoGraph.readReducedGraph("data/processed/" + filename + "_reduced_scheme");
    ofstream file;
    file.open("data/processed/" + filename + "_transitive_closure");
    for (unordered_map<int,vector<int>>::iterator graphIterator = SocialGeoGraph.GraphScheme.begin(); graphIterator != SocialGeoGraph.GraphScheme.end(); graphIterator++){
        int source = graphIterator->first;
        queue<int> Q;
        Q.push(source);
        set<int> orderedVertices;
        while (!Q.empty()){
            int curr = Q.front();
            Q.pop();
            for (int target : SocialGeoGraph.GraphScheme[curr]){
                Q.push(target);
                orderedVertices.insert(target);
            }
        }
        string line = to_string(source); 
        for (int successor : orderedVertices)
            line += "\t" + to_string(successor);
        line += "\n";
        file << line;
    };
}