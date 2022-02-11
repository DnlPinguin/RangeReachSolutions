#include "main.h"


int main(int argc, char **argv){
    string filename, approach, social_method,minimum_bounding_rectangles;
    int numberOfThreads;
    cout << "Enter filename: \n";
    cin >> filename;
    

 


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


    SocialGeoGraph.createBflFileForQuerying("./data/bfl/" + filename + "_graph.sample");
    return 0;

    // Create R-Tree
    clock.start();
    if (minimum_bounding_rectangles == "y"){
	    rTreeSccPlanes* rTreeWithScc = buildTwoDimensionalRtreeWithMbr(&LocationGraph);
    }else{
    	rTreePlanes* rTreeWithScc = buildTwoDimensionalRtree(&LocationGraph);
    }

    rTreeConstructionTime = clock.stop();

    completeConstructionTime = intervalConstructionTime + rTreeConstructionTime + sccConstructiontime;
	ofstream file;
    file.open("data/results/" + filename + "_construction");
	file << "sccConstructiontime" << "\t" << "rTreeConstructionTime" << "\t"<< "intervalConstructionTime" << "\t" << "completeConstructionTime" << "\n";
	file << sccConstructiontime << "\t" << rTreeConstructionTime << "\t"<< intervalConstructionTime << "\t" << completeConstructionTime << "\n";
}