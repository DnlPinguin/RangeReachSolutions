#ifndef GRAPH_H
#define GRAPH_H

#include "../Spatial/MBR.h"
#include "../Spatial/location.h"
#include "../Structures/IntervalScheme.h"
#include "../Spatial/LocationMap.h"
#include "../Structures/boostTypes.h"


#include <queue>
#include <list>
#include <vector>
#include <unordered_set>
#include <set>
#include <map>
#include <iostream>
#include <unordered_map>
#include <stack>
#include <omp.h>


using namespace std;

class Graph {
public:
    // Set of all Vertices
    set<int> V;
    set<int> VReduced;
    int maxNode = INT_MIN;
    //Stores the Postorder of Nodes in the correct order as an array of Ints.
    vector<int> postOrder;
    unordered_map<int, int> postOrderWithIndex;
    unordered_map<int, int> nodeHasPostorder;


    //Representation of Graphs as adjancency list.
    unordered_map <int, vector<int>> GraphScheme;

    //Representation of BiGrahp as adjancency list.
    unordered_map <int, vector<int>> BiGraphScheme;

    //Represenation of Graphs as adjancy list; required for Graph Propagation.
    unordered_map <int, vector<int>> GraphSchemeReverse;

    //All Information for the super connected components.
    unordered_map<int, vector<int>> SuperConnectedComponents;
    unordered_map<int, int> NodeBelongsToSCC;
    unordered_map<int, MBR> SCCsMBR;


    
    //Spanning Tree Represenation of the Graph.
    unordered_map <int, vector<int>> GraphSchemeMST;
    unordered_map <int, vector<int>> GraphSchemeDirectedMST;
    unordered_map <int, vector<int>> GraphSchemeNonSpanningTreeEdges;
    int minimumSpanningTreeRoot;
    int maximumNode = INT_MIN;

    //Interval Scheme Map of the propagated graph.
    unordered_map<int, unordered_set<int>*> AllEdgesGoingIntoKeyNode;
    map <int, vector<IntervalScheme>> IntervalSchemeGraphMap;
    unordered_map <int, boost::dynamic_bitset<>> NodeIntervalBitsetMap;
    map <int, vector<IntervalScheme>> IntervalSchemeGraphMapReverse;

    

    //Basic Graph Constructor
    Graph();

    //Adds Edge between node parentNode and childNode
    void addEdge(int parentNode, int childNode);
    void addSCC(int superNode, int node);
    void addMaximumNode(int node);
    
    //Minimum Spanning Tree creation.
    void createSpanningTree();
    void getNonSpanningTreeEdges(bool reversed);
    void createPostorderTraversal(string fileName);
    vector<int> traverseTree(map<int, bool>* alreadyVisitedPointer);
    bool checkIfStackIsFinished(map<int, bool>* alreadyVisitedPointer);

    //Checks if the given node is reachable depending on the IntervalScheme.
    bool reachNode(int nodeOne, int nodeTwo); 
    bool reachNodeWithBitset(int nodeOne, int nodeTwo);

    //Checks if the given node is reachable using a breadth first search.
    bool reachNodeBFS(int nodeOne, int nodeTwo);

    bool isSuperConnectedComponent(int node);
    int getSuperNode(int node);


    //Topological Sort for Graph
    void topologicalSortUtil(int v, bool visited[], stack<int>& Stack);
    void topologicalSort(vector<int>* topologicalSortedVertices);

    //SCC Creation
    void fillOrder(int v, bool visited[], stack<int>& Stack);
    vector<int> DFSUtilReverse(int v, bool visited[], vector<int> nodesInScc);
    void SCCUtil(int u, int disc[], int low[], stack<int> *st, bool stackMember[]);
    void createSCCsUsingTarjans();

    //Expands the Interval Scheme by adding the not included Edges
    double graphPropagation(string fileName, bool createReverseScheme, LocationMap* SpatialGraph);
    double traditionalGraphPropagation(string filename,bool createReverseScheme);
    double createLabelingSchemeWithOneQueue(string filepath, bool createReverseScheme);
    vector<IntervalScheme>* getReverseIntervalSchemeForNode(int node);



#pragma region Get Statements
    void getAllParents(int root, unordered_set<int>* Ancestors);
    void getAllChildren(int root, unordered_set<int>* Ancestors);
    vector<IntervalScheme> getIntervalSchemeNode(int node);
    map <int, vector<IntervalScheme>> getIntervalSchemeGraphMap();
    int getPostOrderOfNode(int node);
    int getMaximumNode();
    unordered_map<int, vector<int>> getGraphSchemeMst();
    unordered_map<int, vector<int>> getGraphScheme();
    vector<int> getNodes(int node);
    vector<int> getNodesMst(int node);
    vector<int> getPostOrder();
    unordered_map<int, MBR> getSuperConnectedMBRs();
    vector<int> getAllReachableSpatialNodes(int node, LocationMap locGraph);
    vector<int> getAllReachableNodes(int node);
    vector<int> getAllReachableNodesUsingReverseScheme(int node);
    unordered_map<int, vector<int>> getAllSuperConnectedComponents();
    unordered_map<int, vector<int>> getAllSuperConnectedComponentsWithSpatialAttributes();

    vector<int> getSuperConnectedComponentes(int node);
    int getRootNode( );




#pragma endregion


#pragma region print
    void print();
    void printReverse();
    void printPostOrder();
    void printIntervalGraphMap();
    void printMST();
    void printNode(int node);
    void printNodeMst(int node);
    void printSCC();
    void printSccReverse();
    void printMBRWithSCC();
#pragma endregion
    void writeIntervalSchemeIntoFile(string fileName);
    void readIntervalSchemeFromFile(string fileName);
    void writeReducedGraph(string fileName);
    void readReducedGraph(string fileName);
    void readPostorder(string fileName);
    void writeSuperConnectedComponents(string fileName);
    void readSuperConnectedComponents(string fileName);
    void createBflFileForQuerying(string filenName);
};

#endif