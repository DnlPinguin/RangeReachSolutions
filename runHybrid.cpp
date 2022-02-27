#include "main.h"
// #define STATISTICS
    
    
void runHybrid(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging)
{
	Timer clock; 
	rTreeLines rTree = buildThreeDimensionalRtree(SocialGeoGraph, LocationGraph);
	ofstream out(outputFile + "_hybrid_query_result");
	out << "time\tresult\tarea\tdegree\tcardinality\n";

	int counter = 0;
	for (vector<queryParameter>::iterator query = queries->begin(); query != queries->end(); query++)
	{
		int thirdDimensionOfNode = SocialGeoGraph->getPostOrderOfNode(query->queryNode);
		vector<double> results;
		bool result; 

		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = hybridQuery(thirdDimensionOfNode, &rTree, query, &counter);
			results.push_back(clock.stop());
		}
		
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer << " " << result << " " << query->spaceUsed << " " << query->nodeDegree << " " << query->cardinality << " " << counter << endl;
		#else 
			out << fixed << timer << " " << result << " " << query->spaceUsed << " " << query->nodeDegree << " " << query->cardinality << " " << endl;
		#endif
	}
}

void runHybridCube(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging) 
{
	Timer clock;
	rTreeCubes rTree = buildThreeDimensionalRtreeWithCuboids(SocialGeoGraph, LocationGraph);
	ofstream out(outputFile + "_hybrid_mbr_query_result");
	out << "time\tresult\tarea\tdegree\tcardinality\n";
	int counter = 0;
	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		int thirdDimensionOfNode = SocialGeoGraph->getPostOrderOfNode(it->queryNode);
		vector<double> results;
		bool result; 
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = hybridQueryWithCubes(&rTree, it->spatialRegion, LocationGraph, thirdDimensionOfNode, &counter);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << " " << counter << endl;
		#else 
			out << fixed << timer << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << " " << endl;
		#endif	
	}
}

void runReverseHybrid(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging){
	Timer clock;
	rTreePoints rTree = buildThreeDimensionalRtreeWithPoints(SocialGeoGraph, LocationGraph, SocialGeoGraph);
	ofstream out(outputFile + "_hybrid_query_reverse_result");
	out << "time\tresult\tarea\tdegree\tcardinality\n";
	int counter = 0;
	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		vector<IntervalScheme> intervals;
		if (SocialGeoGraph->NodeBelongsToSCC.count(it->queryNode) != 0)
		{
			 intervals = SocialGeoGraph->getIntervalSchemeNode(SocialGeoGraph->NodeBelongsToSCC[it->queryNode]);

		} else {
			 intervals = SocialGeoGraph->getIntervalSchemeNode(it->queryNode);
		}
		vector<double> results;
		bool result; 

		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = hybridQueryReverse(&intervals, &rTree, it, &counter);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << " " << counter << endl;
		#else 
			out << fixed << timer << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << " " << endl;
		#endif


	}
}

void runReverseHybridCube(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging){
	Timer clock;
	rTreeCubes rTree = buildThreeDimesionalRTreeWithPlanes(SocialGeoGraph, LocationGraph, SocialGeoGraph);
	ofstream out(outputFile + "_hybrid_mbr_query_reverse_result");
	int counter = 0;
	out << "time\tresult\tarea\tdegree\tcardinality\n";
	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		vector<IntervalScheme> intervals;
		if (SocialGeoGraph->NodeBelongsToSCC.count(it->queryNode) != 0)
		{
			 intervals = SocialGeoGraph->getIntervalSchemeNode(SocialGeoGraph->NodeBelongsToSCC[it->queryNode]);

		} else {
			 intervals = SocialGeoGraph->getIntervalSchemeNode(it->queryNode);
		}
		vector<double> results;
		bool result; 
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = hybridQueryWithCubesReverse(&intervals, &rTree, it->spatialRegion, LocationGraph , &counter);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << " " << counter << endl;
		#else 
			out << fixed << timer << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << " " << endl;
		#endif	
	}
}

bool requestMbrApproach(){
	cout << "Use Mbr to display spatial data (y/n)";
	string input;
	cin >> input;
	if (input == "y") return true;
	if (input == "n") return false;

	cout << "Im sorry. I didnt understand that \n";
	return requestMbrApproach();
}

bool requestReverseApproach(){
	cout << "Use reverse approach to build r tree and execute queries (y/n)";
	string input;
	cin >> input;
	if (input == "y") return true;
	if (input == "n") return false;

	cout << "Im sorry. I didnt understand that \n";
	return requestReverseApproach();
}

string getFileName(){
	cout << "Enter filename:";
	string input;
	cin >> input;
	return input;
}

int main(int argc, char **argv) {
	vector<queryParameter> queries;
    string superFile;
	bool useMbr, useReverse;
		
	if (argc == 4){
        superFile = argv[1];
        useReverse = strcmp(argv[2], "reverse") == 0 ? true  : false;
        useMbr = strcmp(argv[3], "mbr") == 0 ? true  : false;

	} else {
		superFile = getFileName();
		useReverse = requestReverseApproach();
		useMbr = requestMbrApproach();
	}

	string outputFile = "./data/results/" + superFile;

	queries = readQueries("./data/queries/" + superFile + "_queries");

	Graph SocialGeoGraph;
	LocationMap LocationGraph;


	loadVanillaGeoFileData(superFile, &LocationGraph);
	LocationGraph.printLocations();
	SocialGeoGraph.readSuperConnectedComponents("./data/processed/" + superFile + "_strongly_connected_components");
	SocialGeoGraph.readPostorder("./data/processed/" + superFile + "_postorder");
	LocationGraph.readFileForMap("./data/processed/" + superFile + "_reduced_spatial_data");

	int amount_of_queries_used_for_averging = 1;
	if (useReverse){
		SocialGeoGraph.readIntervalSchemeFromFile("./data/interval_scheme/" + superFile + "_interval_scheme_reverse");
		if (useMbr)
			runReverseHybridCube(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averging);
    	else
        	runReverseHybrid(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averging);

	} else {
		SocialGeoGraph.readIntervalSchemeFromFile("./data/interval_scheme/" + superFile + "_interval_scheme");
		if (useMbr)
			runHybridCube(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averging);
		else
			runHybrid(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averging);
	}
	

	
    




    return 0;
}