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

int main(int argc, char** argv)
{
	string filename, num_of_threads_input, method;

    cout << "Enter filename: \n";
    cin >> filename;

	method = getApproachMethod();

	if (method == "parallel"){
		cout << "Specifiy number of threads \n";
		cin >> num_of_threads_input;
		omp_set_num_threads(stoi(num_of_threads_input));		
	}

	string contrustructionTime; 

	Timer clock;
	Graph SocialGeoGraph;
	LocationMap LocationGraph ;

	// loadFile(filename, &SocialGeoGraph);
	// loadVanillaGeoFileData(filename,&LocationGraph);

	clock.start();
	// SocialGeoGraph.createSCCsUsingTarjans();
	double sccConstructiontime = clock.stop();

	// SocialGeoGraph.writeSuperConnectedComponents(filename);
	// SocialGeoGraph.writeReducedGraph(filename);

	clock.start();
	// SocialGeoGraph.createSpanningTree();
	// SocialGeoGraph.createPostorderTraversal(filename);
	double postorderContrsuctionTime = clock.stop();

	SocialGeoGraph.readReducedGraph("./data/processed/" + filename + "_reduced_scheme");
	SocialGeoGraph.readSuperConnectedComponents("./data/processed/" + filename + "_strongly_connected_components");
	SocialGeoGraph.readPostorder("./data/processed/" + filename + "_postorder");
    // LocationGraph.createMbrs(&SocialGeoGraph.SuperConnectedComponents);
	// LocationGraph.writeMapToFile("./data/processed/" + filename + "_reduced_spatial_data");
	// LocationGraph.readFileForMap("./data/processed/" + filename + "_reduced_spatial_data");
	double timeParallel, timeParallelReverse, timeQueue, timeQueueReverse, timeTraditional, timeTraditionalReverse = 0;

	if (method == "parallel")
	{

		double timeParallel = SocialGeoGraph.graphPropagation("./data/interval_scheme/" + filename + "_interval_scheme", false, &LocationGraph, true);
		double timeParallelReverse = SocialGeoGraph.graphPropagation("./data/interval_scheme/" + filename + "_interval_scheme_reverse", true, &LocationGraph, true);
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
		cin >> num_of_threads_input;
        omp_set_num_threads(stoi(num_of_threads_input));
		double timeParallel = SocialGeoGraph.graphPropagation("./data/interval_scheme/" + filename + "_interval_scheme", false, &LocationGraph, false);
		double timeParallelReverse = SocialGeoGraph.graphPropagation("./data/interval_scheme/" + filename + "_interval_scheme_reverse", true, &LocationGraph, false);
	
	
		ofstream file;
		file.open("data/results/construction/" + filename + "_construction");
		file <<"method" << "\t" << "scc" << "\t" << "postorder" << "\t"<< "indexReverse" << "\t" << "index" << "\n";
		file << "traditional" << "\t" << to_string(sccConstructiontime) << "\t" << to_string(postorderContrsuctionTime) << "\t"<< to_string(timeTraditionalReverse) << "\t" << to_string(timeTraditional) << "\n";
		file << "queue" << "\t" << to_string(sccConstructiontime) << "\t" << to_string(postorderContrsuctionTime) << "\t"<< to_string(timeQueueReverse) << "\t" << to_string(timeQueue) << "\n";
		file << "parallel" << "\t" << to_string(sccConstructiontime) << "\t" << to_string(postorderContrsuctionTime) << "\t"<< to_string(timeParallelReverse) << "\t" << to_string(timeParallel) << "\n";
	}


}


