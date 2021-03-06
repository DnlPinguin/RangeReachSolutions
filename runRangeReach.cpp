#include "main.h"

void runSpaReachQuery(RangeReachVertex* RangeReach, LocationMap* spatialGraph, Graph* SocialGeoGraph, string outputFile, vector<queryParameter>* queries, int layers) 
{
	ofstream out(outputFile + "_spareach");
    Timer clock;
	for (vector<queryParameter>::iterator it = queries->begin(); it != queries->end(); it++)
	{
		clock.start();
		bool result = RangeReach->SpaReachQuery(it->queryNode, it->spatialRegion, SocialGeoGraph, spatialGraph, layers);
		double time = clock.stop();
		out << fixed << time << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << endl;
	}
}

string getFileName(){
	cout << "Enter filename:";
	string input;
	cin >> input;
	return input;
}

int getLayer(){
    cout << "Enter amount of grids in top layer:";
	string input;
	cin >> input;
	return stoi(input);
}

int main(int argc, char **argv) {
	string filename, queryFile;
	int layers;
	if (argc == 3){
		filename = argv[1];
		queryFile = argv[2];
	}else{
		filename = getFileName();
	}

    Graph SocialGeoGraph;
    LocationMap LocationGraph;
    RangeReachVertex RangeReach;


	RangeReach.readAttributesFromFile(filename);
    SocialGeoGraph.readReducedGraph("./data/processed/" + filename + "_reduced_scheme");
    SocialGeoGraph.readSuperConnectedComponents("./data/processed/" + filename + "_strongly_connected_components");
    LocationGraph.readFileForMap("./data/processed/" + filename + "_reduced_spatial_data");
	
	vector<queryParameter> queries = readQueries("./data/queries/" + queryFile);
	int amount_of_queries_used_for_averaging = 1; 
	ofstream out("./data/results/" + filename + "_spareach");
    Timer clock;
	out << "time\tresult\tarea\tdegree\tcardinality\n";

	for (vector<queryParameter>::iterator it = queries.begin(); it != queries.end(); it++)
	{
		vector<double> results;
		bool result;
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = RangeReach.SpaReachQuery(it->queryNode, it->spatialRegion, &SocialGeoGraph, &LocationGraph, layers);
			results.push_back(clock.stop());
		}
		double timer = accumulate( results.begin(), results.end(), 0.0)/results.size(); 
		out << fixed << timer << " " << result << " " << it->spaceUsed << " " << it->nodeDegree << " " << it->cardinality << endl;
	}

    return 0;
}