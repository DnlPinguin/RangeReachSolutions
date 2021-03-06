#include "main.h"
#include "bfl/bfl.h"

#define STATISTICS

void runStrictSpatialFirstQueryUsingBfl(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging){
	cout << "run Strict Spatial First Query Using Bfl" << endl;
	rTreePlanes* rTree = buildTwoDimensionalRtree(LocationGraph);
	Timer clock;
	spatialFirstResult statistics;
	double time;
	ofstream out(outputFile + "_first_bfl_strict_points");
	
	#ifdef STATISTICS
		out << "time\tresult\tarea\tdegree\tcardinality\ttime_spatial\ttime_social\tnodes_inside_query_range\treachability_tests\n";
	#else
		out << "time\tresult\tarea\tdegree\tcardinality\n";
	#endif

	// bfl_index_construction();
	
	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		vector<double> results;
		bool result; 
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result  = strictSpatialFirstQueryUsingBfl(SocialGeoGraph, rTree, it, &statistics);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer <<"\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << "\t" << statistics.time_spatial << "\t" << statistics.time_social << "\t" << statistics.nodes_inside_query_range << "\t" << statistics.number_of_reachability_tests << endl;
		#else 
			out << fixed << timer << "\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << endl;
		#endif
	}


}

void runStrictSpatialFirstQueryUsingBflWithMbr(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging){
	cout << "run Strict Spatial First Query Using Bfl with mbr" << endl;

	Timer clock;
	rTreeSccPlanes* rTreeWithScc = buildTwoDimensionalRtreeWithMbr(LocationGraph);
	
	ofstream out(outputFile + "_first_bfl_strict_mbr");
		
	#ifdef STATISTICS
		out << "time\tresult\tarea\tdegree\tcardinality\ttime_spatial\ttime_social\tnodes_inside_query_range\treachability_tests\n";
	#else
		out << "time\tresult\tarea\tdegree\tcardinality\n";
	#endif

	spatialFirstResult statistics;
	
	// bfl_index_construction();
 
	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		vector<double> results;
		bool result; 
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = strictSpatialFirstQueryUsingBflWithMbr(LocationGraph, SocialGeoGraph, rTreeWithScc, it, &statistics);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer <<"\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << "\t" << statistics.time_spatial << "\t" << statistics.time_social << "\t" << statistics.nodes_inside_query_range << "\t" << statistics.number_of_reachability_tests << endl;
		#else 
			out << fixed << timer << "\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << endl;
		#endif
	}
}

void runSpatialFirstQueryUsingBfl(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging) 
{
	cout << "run Spatial First Query Using Bfl" << endl;

	Timer clock;
	spatialFirstResult statistics;
	rTreePlanes* rTreeWithScc = buildTwoDimensionalRtree(LocationGraph);
	ofstream out(outputFile + "_first_bfl_sequential_points");

		
	#ifdef STATISTICS
		out << "time\tresult\tarea\tdegree\tcardinality\ttime_spatial\ttime_social\tnodes_inside_query_range\treachability_tests\n";
	#else
		out << "time\tresult\tarea\tdegree\tcardinality\n";
	#endif

	// bfl_index_construction();


	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		vector<double> results;
		bool result; 
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = spatialFirstQueryUsingBfl(SocialGeoGraph, rTreeWithScc, it, &statistics);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << endl;
		#else 
			out << fixed << timer << "\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << endl;
		#endif
	}

}

void runSpatialFirstMbrQueryUsingBfl(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging) 
{
	cout << "run Spatial First Query Using Bfl With Mbr" << endl;

	Timer clock;
	rTreeSccPlanes* rTreeWithScc = buildTwoDimensionalRtreeWithMbr(LocationGraph);
	
	ofstream out(outputFile + "_first_bfl_sequential_mbr");
		
	#ifdef STATISTICS
		out << "time\tresult\tarea\tdegree\tcardinality\ttime_spatial\ttime_social\tnodes_inside_query_range\treachability_tests\n";
	#else
		out << "time\tresult\tarea\tdegree\tcardinality\n";
	#endif

	spatialFirstResult statistics;
	
	// bfl_index_construction();


	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		vector<double> results;
		bool result; 
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = spatialFirstQueryUsingBflWithMbr(LocationGraph, SocialGeoGraph, rTreeWithScc, it, &statistics);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << endl;
		#else 
			out << fixed << timer << "\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << endl;
		#endif
	}
}

void runSpatialFirstQuery(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging) 
{
	cout << "Run spatial first query" << endl;

	Timer clock;
	spatialFirstResult statistics;
	rTreePlanes* rTreeWithScc = buildTwoDimensionalRtree(LocationGraph);
	ofstream out(outputFile + "_first_sequential_points_interval");
		
	#ifdef STATISTICS
		out << "time\tresult\tarea\tdegree\tcardinality\ttime_spatial\ttime_social\tnodes_inside_query_range\treachability_tests\n";
	#else
		out << "time\tresult\tarea\tdegree\tcardinality\n";
	#endif

	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		vector<double> results;
		bool result; 
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = spatialFirstQuery(SocialGeoGraph, rTreeWithScc, it, &statistics);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << endl;
		#else 
			out << fixed << timer << "\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << endl;
		#endif
	}

}

void runSpatialFirstMbrQuery(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging) 
{
	Timer clock;
	rTreeSccPlanes* rTreeWithScc = buildTwoDimensionalRtreeWithMbr(LocationGraph);
	ofstream out(outputFile + "_first_sequential_mbr_interval");

	
	#ifdef STATISTICS
		out << "time\tresult\tarea\tdegree\tcardinality\ttime_spatial\ttime_social\tnodes_inside_query_range\treachability_tests\n";
	#else
		out << "time\tresult\tarea\tdegree\tcardinality\n";
	#endif

	spatialFirstResult statistics;

	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		vector<double> results;
		bool result; 
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = spatialFirstQueryWithMbr(LocationGraph, SocialGeoGraph, rTreeWithScc, it, &statistics);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << endl;
		#else 
			out << fixed << timer << "\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << endl;
		#endif
	}
}

void runStrictSpatialFirstQuery(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging)
{
	rTreePlanes* rTree = buildTwoDimensionalRtree(LocationGraph);
	Timer clock;
	ofstream out(outputFile + "_first_strict_points_interval");
	spatialFirstResult statistics;

	
	#ifdef STATISTICS
		out << "time\tresult\tarea\tdegree\tcardinality\ttime_spatial\ttime_social\tnodes_inside_query_range\treachability_tests\n";
	#else
		out << "time\tresult\tarea\tdegree\tcardinality\n";
	#endif

	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		vector<double> results;
		bool result; 
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result  = strictSpatialFirstQuery(SocialGeoGraph, rTree, it, &statistics);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer <<"\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << "\t" << statistics.time_spatial << "\t" << statistics.time_social << "\t" << statistics.nodes_inside_query_range << "\t" << statistics.number_of_reachability_tests << endl;
		#else 
			out << fixed << timer << "\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << endl;
		#endif
	}
}

void runStrictSpatialFirstMbrQuery(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging)
{
	Timer clock;
	rTreeSccPlanes* rTreeWithScc = buildTwoDimensionalRtreeWithMbr(LocationGraph);
	ofstream out(outputFile + "_first_strict_mbr_interval");
	spatialFirstResult statistics;

	
	#ifdef STATISTICS
		out << "time\tresult\tarea\tdegree\tcardinality\ttime_spatial\ttime_social\tnodes_inside_query_range\treachability_tests\n";
	#else
		out << "time\tresult\tarea\tdegree\tcardinality\n";
	#endif

	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		vector<double> results;
		bool result; 
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = strictSpatialFirstQueryWithMbr(LocationGraph, SocialGeoGraph, rTreeWithScc, it, &statistics);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 

		#ifdef STATISTICS
			out << fixed << timer <<"\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << "\t" << statistics.time_spatial << "\t" << statistics.time_social << "\t" << statistics.nodes_inside_query_range << "\t" << statistics.number_of_reachability_tests << endl;
		#else 
			out << fixed << timer << "\t" << result << "\t" << it->spaceUsed << "\t" << it->nodeDegree << "\t" << it->cardinality << endl;
		#endif
	}
}

bool requestMbrApproach(){
	cout << "Use Mbr to display spatial data (y/n)";
	string input;
	cin >> input;
	if (input == "y") return true;
	if (input == "n") return false;
	return requestMbrApproach();
}

bool requestStrictApproach(){
	cout << "Use strict approach to answer spatial tests (y/n)";
	string input;
	cin >> input;
	if (input == "y") return true;
	if (input == "n") return false;
	return requestStrictApproach();
}

bool requestBfl(){
	cout << "Use BFL to answer reachability  (y/n)";
	string input;
	cin >> input;
	if (input == "y") return true;
	if (input == "n") return false;
	return requestMbrApproach();
}

string getFileName(){
	cout << "Enter filename:";
	string input;
	cin >> input;
	return input;
}

int main(int argc, char **argv) {
    string superFile, queryFile;
	bool useMbr, useStrict, useBfl;
	if (argc == 6){
        superFile = argv[1];
        useStrict = strcmp(argv[2], "strict") == 0 ? true  : false;
        useMbr = strcmp(argv[3], "mbr") == 0 ? true  : false;
		useBfl = strcmp(argv[4], "bfl") == 0 ? true : false;
		queryFile = argv[5];
	} else {
		superFile = getFileName();
		useStrict = requestStrictApproach();
		useMbr = requestMbrApproach();
		useBfl = requestBfl();
	}




	string outputFile = "data/results/" + superFile;
	vector<queryParameter> queries = readQueries("data/queries/" + queryFile);


    Graph SocialGeoGraph;
    LocationMap LocationGraph;
    
     
	if (useBfl){

		string bflFileName = "data/bfl/" + superFile + "_graph"; 
		SocialGeoGraph.readBflForNodeIdentifer("data/bfl/" + superFile + "_bfl_id");
		read_graph_for_bfl(bflFileName.c_str());
		bfl_index_construction();
	}
    SocialGeoGraph.readReducedGraph("data/processed/" + superFile + "_reduced_scheme");
	SocialGeoGraph.readSuperConnectedComponents("data/processed/" + superFile + "_strongly_connected_components");
	SocialGeoGraph.readPostorder("data/processed/" + superFile + "_postorder");
	SocialGeoGraph.readIntervalSchemeFromFile("data/interval_scheme/" + superFile + "_interval_scheme");
	LocationGraph.readFileForMap("data/processed/" + superFile + "_reduced_spatial_data");
    
	int amount_of_queries_used_for_averaging = 1;

	if (useBfl  && useStrict && useMbr)
		runStrictSpatialFirstQueryUsingBflWithMbr(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averaging);

	if (useBfl  && useStrict && !useMbr)
		runStrictSpatialFirstQueryUsingBfl(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averaging);

	if (useBfl  && !useStrict && useMbr)
		runSpatialFirstMbrQueryUsingBfl(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averaging);

	if (useBfl  && !useStrict && !useMbr)
		runSpatialFirstQueryUsingBfl(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averaging);

	if (!useBfl  && useStrict && useMbr)
		runStrictSpatialFirstMbrQuery(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averaging);

	if (!useBfl  && useStrict && !useMbr)
		runStrictSpatialFirstQuery(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averaging);

	if (!useBfl  && !useStrict && useMbr)
		runSpatialFirstMbrQuery(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averaging);

	if (!useBfl  && !useStrict && !useMbr)
		runSpatialFirstQuery(&SocialGeoGraph, &LocationGraph, outputFile, &queries, amount_of_queries_used_for_averaging);
    
	
    return 0;
}
