#include "main.h"
#define STATISTICS 


void runSocialFirstQuery(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging)
{
	ofstream out(outputFile + "_social_first_sequential_points");
	Timer clock;
	socialFirstResult statistics;
	vector<double> results;
	bool result; 

	for (vector<queryParameter>::iterator qIter = queries->begin(); qIter != queries->end(); qIter++)
	{
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++)
		{
			clock.start();
			vector<IntervalScheme>*  Intervals = &(SocialGeoGraph->IntervalSchemeGraphMap[qIter->queryNode]);
			result = socialFirstQuery(SocialGeoGraph, LocationGraph, qIter, &statistics);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 
		#ifdef STATISTICS
			out << fixed << timer << "\t" << result << "\t" << qIter->spaceUsed << "\t" << qIter->nodeDegree << "\t" << qIter->cardinality <<  "\t" << statistics.time_social << "\t" <<  statistics.time_spatial << "\t" << statistics.reachable_nodes << "\t" <<  statistics.number_of_spatial_range_tests << endl;
		#else
			out << fixed << timer << "\t" << result << "\t" << qIter->spaceUsed << "\t" << qIter->nodeDegree << "\t" << qIter->cardinality <<  endl;
		#endif
	}
}

void runSocialFirstMbrQuery(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging)
{
	ofstream out(outputFile + "_social_first_sequential_mbr");
	socialFirstResult statistics;
	Timer clock;

	vector<double> results;
	bool result; 

	for (vector<queryParameter>::iterator qIter = queries->begin(); qIter != queries->end(); qIter++)
	{
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++)
		{
			clock.start();
			result = socialFirstQueryWithMbr(SocialGeoGraph, LocationGraph, qIter,  &statistics);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 
		#ifdef STATISTICS
			out << fixed << timer << "\t" << result << "\t" << qIter->spaceUsed << "\t" << qIter->nodeDegree << "\t" << qIter->cardinality <<  "\t" << statistics.time_social << "\t" <<  statistics.time_spatial << "\t" << statistics.reachable_nodes << "\t" <<  statistics.number_of_spatial_range_tests << endl;
		#else
			out << fixed << timer << "\t" << result << "\t" << qIter->spaceUsed << "\t" << qIter->nodeDegree << "\t" << qIter->cardinality <<  endl;
		#endif
	}
}

void runStrictSocialFirstQuery(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging)
{
	ofstream out(outputFile + "_social_first_strict_points");
	Timer clock;
	socialFirstResult statistics;

	vector<double> results;
	bool result;

	for (vector<queryParameter>::iterator qIter = queries->begin(); qIter != queries->end(); qIter++)
	{
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++)
		{
			clock.start();
			result = socialFirstQuery(SocialGeoGraph, LocationGraph, qIter, &statistics);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 
		#ifdef STATISTICS
			out << fixed << timer << "\t" << result << "\t" << qIter->spaceUsed << "\t" << qIter->nodeDegree << "\t" << qIter->cardinality <<  "\t" << statistics.time_social << "\t" <<  statistics.time_spatial << "\t" << statistics.reachable_nodes << "\t" <<  statistics.number_of_spatial_range_tests << endl;
		#else
			out << fixed << timer << "\t" << result << "\t" << qIter->spaceUsed << "\t" << qIter->nodeDegree << "\t" << qIter->cardinality <<  endl;
		#endif
	}
}

void runStrictSocialFirstMbrQuery(Graph* SocialGeoGraph, LocationMap* LocationGraph, string outputFile, vector<queryParameter>* queries, int amount_of_queries_used_for_averaging)
{
	ofstream out(outputFile + "_social_first_mbr_strict");
	Timer clock;
	socialFirstResult statistics;

	vector<double> results;
	bool result; 

	for (vector<queryParameter>::iterator qIter = queries->begin(); qIter != queries->end(); qIter++)
	{
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = socialFirstQueryWithMbr(SocialGeoGraph, LocationGraph, qIter, &statistics);
			results.push_back(clock.stop());
		}
		double timer;
		#ifdef STATISTICS
			out << fixed << timer << "\t" << result << "\t" << qIter->spaceUsed << "\t" << qIter->nodeDegree << "\t" << qIter->cardinality <<  "\t" << statistics.time_social << "\t" <<  statistics.time_spatial << "\t" << statistics.reachable_nodes << "\t" <<  statistics.number_of_spatial_range_tests << endl;
		#else
			out << fixed << timer << "\t" << result << "\t" << qIter->spaceUsed << "\t" << qIter->nodeDegree << "\t" << qIter->cardinality <<  endl;
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
	return requestMbrApproach();
}

string getFileName(){
	cout << "Enter filename:";
	string input;
	cin >> input;
	return input;
}

int main(int argc, char **argv) {

	string superFile;
	bool useStrict, useMbr;

	if (argc == 4){
        superFile = argv[1];
		useStrict = strcmp(argv[2], "strict") == 0 ? true  : false;
        useMbr = strcmp(argv[3], "mbr") == 0 ? true  : false;
	} else {
		superFile = getFileName();
		useStrict = requestStrictApproach();
		useMbr = requestMbrApproach();
	}


	string outputFile = "./data/results/" + superFile;
	vector<queryParameter>* queries = readQueries("./data/queries/" + superFile + "_queries");



    Graph SocialGeoGraph;
    LocationMap LocationGraph;
     
    SocialGeoGraph.readReducedGraph("./data/processed/" + superFile + "_reduced_scheme");
	SocialGeoGraph.readSuperConnectedComponents("./data/processed/" + superFile + "_strongly_connected_components");
	SocialGeoGraph.readPostorder("./data/processed/" + superFile + "_postorder");
	SocialGeoGraph.readIntervalSchemeFromFile("./data/interval_scheme/" + superFile + "_interval_scheme_reverse");
	LocationGraph.readFileForMap("./data/processed/" + superFile + "_reduced_spatial_data");

	int amount_of_queries_used_for_averaging = 1;

	for (vector<queryParameter>::iterator qIter = queries->begin(); qIter != queries->end(); qIter++){
		if (useMbr && useStrict)
			runStrictSocialFirstMbrQuery(&SocialGeoGraph, &LocationGraph, outputFile, queries, amount_of_queries_used_for_averaging);

		if (useMbr && !useStrict)
			runSocialFirstMbrQuery(&SocialGeoGraph, &LocationGraph, outputFile, queries, amount_of_queries_used_for_averaging);

		if (!useMbr && useStrict)
			runStrictSocialFirstQuery(&SocialGeoGraph, &LocationGraph, outputFile, queries, amount_of_queries_used_for_averaging);

		if (!useMbr && !useStrict)
			runSocialFirstQuery(&SocialGeoGraph, &LocationGraph, outputFile, queries , amount_of_queries_used_for_averaging);
	}
    return 0;
}
