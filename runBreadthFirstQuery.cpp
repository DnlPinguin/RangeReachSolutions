#include "main.h"
// #define STATISTICS
    


bool runBfsQuery(Graph* SocialGeoGraph, LocationMap* LocationGraph, int queryNode, MBR queryWindow){
    queue<int> Q;
    int node = queryNode;
    if (SocialGeoGraph->NodeBelongsToSCC.count(queryNode)){
            node = SocialGeoGraph->NodeBelongsToSCC[queryNode];
    }
    Q.push(node);

    while (!Q.empty()){
        int curr_node = Q.front();
        Q.pop();

        if (curr_node != queryNode){
            if (LocationGraph->existLocation(curr_node)){
                if(SocialGeoGraph->SuperConnectedComponents.count(curr_node) != 0){
                    for (int component_node : SocialGeoGraph->SuperConnectedComponents[curr_node])
                    {
                        if (LocationGraph->LocationScheme.count(component_node) != 0){
                            if (queryWindow.containsPoint(LocationGraph->LocationScheme[component_node])){
                                return true;
                            }
                        }
                    }
                } 
                else 
                {
                    if (queryWindow.containsPoint(LocationGraph->LocationScheme[curr_node])){
                        return true;
                    }
                }
            } 
        }
        for (int next_node : SocialGeoGraph->GraphScheme[curr_node]){
            Q.push(next_node);
        }
    }
    return false;
}

string getFileName(){
	cout << "Enter filename:";
	string input;
	cin >> input;
	return input;
}

int main(int argc, char **argv) {
    string superFile, queryFile;
	if (argc == 3){
        superFile = argv[1];
        queryFile = argv[2];
	} else {
		superFile = getFileName();
	}

	string outputFile = "./data/results/" + superFile + "_bfs";
	vector<queryParameter> queries = readQueries("./data/queries/" + queryFile);

	Graph SocialGeoGraph;
	LocationMap LocationGraph;

    loadVanillaGeoFileData(superFile, &LocationGraph);
    SocialGeoGraph.readReducedGraph("./data/processed/" + superFile + "_reduced_scheme");
	SocialGeoGraph.readSuperConnectedComponents("./data/processed/" + superFile + "_strongly_connected_components");
	SocialGeoGraph.readPostorder("./data/processed/" + superFile + "_postorder");
	LocationGraph.readFileForMap("./data/processed/" + superFile + "_reduced_spatial_data");
	int amount_of_queries_used_for_averging = 1;
    ofstream out(outputFile);
    out << "time\tresult\tarea\tdegree\tcardinality\n";

    Timer clock;

    for(auto query : queries)
    {

        box spatialWindow = query.spatialRegion;
        MBR queryWindowToMBR(
            query.spatialRegion.min_corner().get<0>(),
            query.spatialRegion.min_corner().get<1>(),
            query.spatialRegion.max_corner().get<0>(),
            query.spatialRegion.max_corner().get<1>()
        );
        clock.start();
        bool result = runBfsQuery(&SocialGeoGraph, &LocationGraph, query.queryNode, queryWindowToMBR);
        out << fixed << clock.stop() << " " << result << " " << query.spaceUsed << " " << query.nodeDegree << " " << query.cardinality << " " << endl;

    }

    return 0;
}