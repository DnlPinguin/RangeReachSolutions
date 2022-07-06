#include "main.h"


int main(int argc, char **argv){
    string filename, approach, minimum_bounding_rectangles;
    int numberOfThreads;
    cout << "Enter filename: \n";
    cin >> filename;
    
    cout << "Use parallelizied appraoch y/n: \n";
    cin >> approach;

    cout << "Speciify number of threads: \n";
    cin >> numberOfThreads;

    cout << "use mimimum bounding rectangles (y/n)\n"; 
    cin  >> minimum_bounding_rectangles; 

    Timer clock, rTreeClock, stronglyConnectedComponentsClock;
	Graph SocialGeoGraph;
	LocationMap LocationGraph;

    loadFile(filename, &SocialGeoGraph);
	loadVanillaGeoFileData(filename, &LocationGraph);


    double completeConstructionTime, rTreeConstructionTime, sccConstructiontime, intervalConstructionTime;
    
    clock.start();
    SocialGeoGraph.createSCCsUsingTarjans();
	sccConstructiontime  = clock.stop();

    clock.start();
    LocationGraph.createMbrs(&SocialGeoGraph.SuperConnectedComponents);
    sccConstructiontime = clock.stop();
	LocationGraph.writeMapToFile("./data/processed/" + filename + "_reduced_spatial_data");

	
	clock.start();
    SocialGeoGraph.createSpanningTree();
	SocialGeoGraph.createPostorderTraversal(filename);
    string filepath=  "./data/interval_scheme/" + filename + "_interval_scheme_";
    SocialGeoGraph.graphPropagation(filepath, false, &LocationGraph, 0, 1);
	intervalConstructionTime = clock.stop();


    // Create R-Tree
    clock.start();
    if (minimum_bounding_rectangles == "y"){
	    rTreeCubes rTree = buildThreeDimensionalRtreeWithCuboids(&SocialGeoGraph, &LocationGraph);
    }else{
        rTreeLines rTree = buildThreeDimensionalRtree(&SocialGeoGraph, &LocationGraph);
    }

    rTreeConstructionTime = clock.stop();

    completeConstructionTime = intervalConstructionTime + rTreeConstructionTime + sccConstructiontime;
	ofstream file;
    file.open("data/results/" + filename + "_construction");
	file << "sccConstructiontime" << "\t" << "rTreeConstructionTime" << "\t"<< "intervalConstructionTime" << "\t" << "completeConstructionTime" << "\n";
	file << sccConstructiontime << "\t" << rTreeConstructionTime << "\t"<< intervalConstructionTime << "\t" << completeConstructionTime << "\n";
}