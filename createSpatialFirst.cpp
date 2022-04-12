#include "main.h"


int main(int argc, char **argv){
    string filename, approach, social_method,minimum_bounding_rectangles;
    int numberOfThreads;
    cout << "Enter filename: \n";
    cin >> filename; 

    Timer clock, rTreeClock, stronglyConnectedComponentsClock;
	Graph SocialGeoGraph;
	LocationMap LocationGraph;


    double completeConstructionTime, rTreeConstructionTime, sccConstructiontime, intervalConstructionTime;

    SocialGeoGraph.readReducedGraph("data/processed/" + filename + "_reduced_scheme");

    SocialGeoGraph.createBflFileForQuerying(filename);
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