#include "main.h"

int main(int argc, char **argv){
    string filename;

    if (argc == 2){
        filename = argv[1];
    }  else {
        cout << "Enter filename: \n";
        cin >> filename; 
    }
    Timer clock;
	Graph SocialGeoGraph;
	LocationMap LocationGraph;



    SocialGeoGraph.readReducedGraph("data/processed/" + filename + "_transitive_closure");

    unordered_map<int,vector<int>>::iterator graphIterator;

    string filepath = "./data/bfl/" + filename + "_graph.sample";
    string filepathForBflId = "./data/bfl/" + filename + "_bfl_id";
    
    ofstream fileGraph;
    ofstream fileBflID;

    fileGraph.open(filepath);
    fileBflID.open(filepathForBflId);

    fileGraph << "graph_for_greach\n";
    fileGraph << SocialGeoGraph.V.size()  << "\n";

    int counter = 0;
    for ( graphIterator = SocialGeoGraph.GraphScheme.begin(); graphIterator != SocialGeoGraph.GraphScheme.end(); graphIterator++){
        int source = graphIterator->first;
        string line;
        if (SocialGeoGraph.nodeToBflIdentifier.count(source) == 0){
            SocialGeoGraph.nodeToBflIdentifier[source] = counter;
            fileBflID << source << "\t" << counter << "\n";
            counter++;
        }
        fileGraph << SocialGeoGraph.nodeToBflIdentifier[source] << ":";
        
        for (int target : SocialGeoGraph.GraphScheme[source]){
            if (SocialGeoGraph.nodeToBflIdentifier.count(target) == 0){
                SocialGeoGraph.nodeToBflIdentifier[target] = counter;
                fileBflID << target << "\t" << counter << "\n";
                counter++;

            }
            fileGraph << " " << SocialGeoGraph.nodeToBflIdentifier[target];
        }
        fileGraph << "\n";
    };
}