#include "main.h"

string getApproachMethod(){
	bool use_parallelized_approach;
	string possibleAnswers[] = {"parallel", "queue", "traditional", "compareAll"};
	string answer;
	cout << "choose approach (parallel/queue/traditional/compareAll)?\n";
	cin >> answer;

	if (find(begin(possibleAnswers), end(possibleAnswers), answer) != end(possibleAnswers))
		return answer;

	cout << "Answer not recognized. Choose parallelized approach y/n ? \n";
	return getApproachMethod();
}



// NOTE: Traditional and Queue are strictly limited by processing memory. Not working on private machines.

// Create Interval scheme
// To execute the creation:
//
//
// To execute on multiple machines:
// Executables/createIntervalScheme {{filename}} parallel {{number of threads}} {{machineId}} {{total number of machines}}

int main(int argc, char** argv)
{
	string filename, method;
	int numOfThreads, machineId, totalNumberOfMachines;

	if (argc != 6){
		cout << "Only supporting parallel now, other methods are not up to date" << endl;
		return 0 ;
	}	

	filename = argv[1];
	method = argv[2];
	numOfThreads = stoi(argv[3]) - 1;
	omp_set_num_threads(numOfThreads);
	machineId = stoi(argv[4]);
	totalNumberOfMachines = stoi(argv[5]);

	if (machineId >= totalNumberOfMachines){
		cout << "machineId is exceeding allowed number of total machines" << endl;
		return 0 ;
	}

	Timer clock;
	Graph SocialGeoGraph;
	LocationMap LocationGraph;

	SocialGeoGraph.readReducedGraph("./data/processed/" + filename + "_reduced_scheme");
	SocialGeoGraph.readSuperConnectedComponents("./data/processed/" + filename + "_strongly_connected_components");
	SocialGeoGraph.readPostorder("./data/processed/" + filename + "_postorder");
	double timeParallel, timeParallelReverse, timeQueue, timeQueueReverse, timeTraditional, timeTraditionalReverse = 0, sccConstructiontime = 0, postorderContrsuctionTime = 0;


	if (method == "parallel")
	{
		double timeParallel = SocialGeoGraph.graphPropagation("./data/interval_scheme/" + filename + "_interval_scheme", false, &LocationGraph, machineId, totalNumberOfMachines);
		double timeParallelReverse = SocialGeoGraph.graphPropagation("./data/interval_scheme/" + filename + "_interval_scheme_reverse", true, &LocationGraph, machineId, totalNumberOfMachines);
	}
		else if (method == "queue")
	{
		double timeQueue = SocialGeoGraph.createLabelingSchemeWithOneQueue("./data/interval_scheme/" + filename + "_interval_scheme", false);
		double timeQueueReverse = SocialGeoGraph.createLabelingSchemeWithOneQueue("./data/interval_scheme/" + filename + "_interval_scheme_reverse", true);
	} 
		else if (method == "traditional")
	{
		SocialGeoGraph.getNonSpanningTreeEdges(false);
		double timeTraditional = SocialGeoGraph.traditionalGraphPropagation("./data/interval_scheme/" + filename + "_interval_scheme", false);
		SocialGeoGraph.getNonSpanningTreeEdges(true);
		double timeTraditionalReverse = SocialGeoGraph.traditionalGraphPropagation("./data/interval_scheme/" + filename + "_interval_scheme_reverse", true);
	}
		else if (method == "compareAll")
	{
		SocialGeoGraph.getNonSpanningTreeEdges(false);
		double timeTraditional = SocialGeoGraph.traditionalGraphPropagation("./data/interval_scheme/" + filename + "_interval_scheme", false);
		SocialGeoGraph.getNonSpanningTreeEdges(true);
		double timeTraditionalReverse = SocialGeoGraph.traditionalGraphPropagation("./data/interval_scheme/" + filename + "_interval_scheme_reverse", true);
	
		double timeQueue = SocialGeoGraph.createLabelingSchemeWithOneQueue("./data/interval_scheme/" + filename + "_interval_scheme", false);
		double timeQueueReverse = SocialGeoGraph.createLabelingSchemeWithOneQueue("./data/interval_scheme/" + filename + "_interval_scheme_reverse", true);
		
		cout << "Specifiy number of threads \n";
		cin >> numOfThreads;
        omp_set_num_threads(numOfThreads);
		double timeParallel = SocialGeoGraph.graphPropagation("./data/interval_scheme/" + filename + "_interval_scheme", false, &LocationGraph, machineId, totalNumberOfMachines);
		double timeParallelReverse = SocialGeoGraph.graphPropagation("./data/interval_scheme/" + filename + "_interval_scheme_reverse", true, &LocationGraph, machineId, totalNumberOfMachines);
	
		ofstream file;
		file.open("data/results/construction/" + filename + "_construction");
		file <<"method" << "\t" << "scc" << "\t" << "postorder" << "\t"<< "indexReverse" << "\t" << "index" << "\n";
		file << "traditional" << "\t" << to_string(sccConstructiontime) << "\t" << to_string(postorderContrsuctionTime) << "\t"<< to_string(timeTraditionalReverse) << "\t" << to_string(timeTraditional) << "\n";
		file << "queue" << "\t" << to_string(sccConstructiontime) << "\t" << to_string(postorderContrsuctionTime) << "\t"<< to_string(timeQueueReverse) << "\t" << to_string(timeQueue) << "\n";
		file << "parallel" << "\t" << to_string(sccConstructiontime) << "\t" << to_string(postorderContrsuctionTime) << "\t"<< to_string(timeParallelReverse) << "\t" << to_string(timeParallel) << "\n";
	}
}


