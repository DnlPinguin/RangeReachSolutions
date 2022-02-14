#include "graph.h"
#include <math.h>
#include <exception>

#pragma region Constructor

Graph::Graph() {}

void Graph::addMaximumNode(int node) 
{
    if (node > this->maximumNode){ this->maximumNode = node; }

}

void Graph::addEdge(int parentNode, int childNode) {
    this->V.insert(parentNode);
    this->V.insert(childNode);

    this->GraphSchemeReverse[childNode].push_back(parentNode);
    this->GraphScheme[parentNode].push_back(childNode);
    this->BiGraphScheme[parentNode].push_back(childNode);
    this->BiGraphScheme[childNode].push_back(parentNode);
}

void Graph::addSCC(int superNode, int node)
{
    this->SuperConnectedComponents[superNode].push_back(node);
    this->NodeBelongsToSCC[node] = superNode;
}

void Graph::getNonSpanningTreeEdges(bool reversed)
{
    GraphSchemeDirectedMST.clear();
    GraphSchemeNonSpanningTreeEdges.clear();
    for (unordered_map <int, vector<int>>::iterator iter = GraphScheme.begin(); iter != GraphScheme.end(); iter++){
        int sourceNode = iter->first;
        for (int targetNode : iter->second){
            if (find(GraphSchemeMST[sourceNode].begin(), GraphSchemeMST[sourceNode].end(), targetNode)!= GraphSchemeMST[sourceNode].end()){
                if (reversed){
                    GraphSchemeDirectedMST[targetNode].push_back(sourceNode);
                }else{
                   GraphSchemeDirectedMST[sourceNode].push_back(targetNode);
                }
            }else{
                if (reversed){
                    GraphSchemeNonSpanningTreeEdges[targetNode].push_back(sourceNode);
                }else{
                    GraphSchemeNonSpanningTreeEdges[sourceNode].push_back(targetNode);
                }
            }
        }
    }
}

void Graph::createSpanningTree() 
{
    map<int, bool> visited;
    map<int, bool> visitedByEdge;
    queue<int> Q;

    unordered_map<int, vector<int>>::iterator it;
    
    // unordered_set<int> alreadyVisited;
    // for (it = GraphScheme.begin(); it != GraphScheme.end(); it++)
    // {
    //     int sourceNode = it->first;
    //     for (int targetNode : it->second){
    //         if (alreadyVisited.count(targetNode) == 0)
    //     }
    // }

    pair<int, int> maxNode = make_pair(0, 0);
    for (it = GraphScheme.begin(); it != GraphScheme.end(); it++)
    {
        if (maxNode.second < it->second.size())
        {
            maxNode = make_pair(it->first, it->second.size());
        }
    }

    this->minimumSpanningTreeRoot = maxNode.first;
    Q.push(this->minimumSpanningTreeRoot);

    while(!Q.empty())
    {
        int next_node = Q.front();
        visited[next_node] = true;
        Q.pop();
        for (int iter : BiGraphScheme[next_node])
        {
            if(visited[iter] == false)
            {
                if (!visitedByEdge[iter])
                {
                    GraphSchemeMST[next_node].push_back(iter);
                    visitedByEdge[iter] = true;
                    Q.push(iter);
                }
                
            }
        }
    }
    BiGraphScheme.clear();
    cout << "   spanning tree created!" << endl;
}

#pragma endregion

#pragma region Get Statements
vector<int> Graph::getPostOrder() {
    return this->postOrder;
}

vector<int> Graph::getNodes(int node) {
    if (GraphScheme.count(node) != 0) {
        return GraphScheme[node];
    }
    return { 0 };
}

vector<int> Graph::getNodesMst(int node) {
    return GraphSchemeMST[node];
}

vector<int> Graph::getAllReachableSpatialNodes(int node, LocationMap locGraph) {
    vector<int> reachableNodesWithSpatial;
    vector<int> reachableNodes = getAllReachableNodes(node);

    for (int nodeReachable : reachableNodes)
    {
        if (locGraph.existLocation(nodeReachable))
        {
            reachableNodesWithSpatial.push_back(nodeReachable);
        }
    }
    return reachableNodesWithSpatial;
}

// BUILD IT TO SET, SO we can perform a range Reach query using pre, post Scheme as Vertices
vector<int> Graph::getAllReachableNodesUsingReverseScheme(int node)
{
    vector<IntervalScheme> currNodeInterval = this->IntervalSchemeGraphMap[node];
    vector<IntervalScheme>::iterator iter; 
    vector<int> reachableNodes;
    for (iter = currNodeInterval.begin() ; iter != currNodeInterval.end(); iter++)
    {
        for (int it = iter->pre; it <= iter->post; it++) 
        {
            reachableNodes.push_back(nodeHasPostorder[it]);
        }
    }
    return reachableNodes;
}

vector<int> Graph::getAllReachableNodes(int node) {
    vector<int> reachableNodes;
    map <int, vector<IntervalScheme>>::iterator it;
    for (it = IntervalSchemeGraphMap.begin(); it != IntervalSchemeGraphMap.end(); it++) {
        for (auto intervalIterator : it->second) {
            if (postOrderWithIndex[node] >= intervalIterator.pre && postOrderWithIndex[node] <= intervalIterator.post) 
            {
                reachableNodes.push_back(it->first);
            }
        }
    }
    return reachableNodes;
}

int Graph::getPostOrderOfNode(int node) {
    if (NodeBelongsToSCC.count(node) != 0) {
        node = NodeBelongsToSCC[node];
    }
    return this->postOrderWithIndex[node];
}

unordered_map<int, vector<int>> Graph::getGraphSchemeMst() {
    return this->GraphSchemeMST;
};

unordered_map<int, vector<int>> Graph::getGraphScheme() {
    return this->GraphScheme;
};

unordered_map<int, vector<int>> Graph::getAllSuperConnectedComponents() {
    return SuperConnectedComponents;
}

map <int, vector<IntervalScheme>> Graph::getIntervalSchemeGraphMap()
{
    return this->IntervalSchemeGraphMap;
}

vector<IntervalScheme> Graph::getIntervalSchemeNode(int node)
{
    return this->IntervalSchemeGraphMap[node] ;
}

unordered_map<int, MBR> Graph::getSuperConnectedMBRs()
{
    return SCCsMBR;
}
#pragma endregion

#pragma region Operations On Graph
bool Graph::isSuperConnectedComponent(int node)
{
    if (SuperConnectedComponents.count(node) != 0) {
        return true;
    }
    return false;
}

int Graph::getSuperNode(int node) {
    return NodeBelongsToSCC[node];
}

int Graph::getRootNode() {
    return this->minimumSpanningTreeRoot;
}

int Graph::getMaximumNode() {
    return this->maximumNode;
}

/*
Checks with the Interval Scheme node One is able to reach node Two
*/
bool Graph::reachNode(int nodeOne, int nodeTwo) {
    if (NodeBelongsToSCC.count(nodeOne) != 0) {
        nodeOne = NodeBelongsToSCC[nodeOne];
    }
    if (nodeOne == nodeTwo) {
        return false;
    }
    for (auto it : IntervalSchemeGraphMap[nodeTwo]) 
    {
        if (postOrderWithIndex[nodeOne] >= it.pre && postOrderWithIndex[nodeOne] <= it.post)
        {
            return true;
        }
    }
    return false;
}

/*
Checks with the Interval Scheme node One is able to reach node Two
*/
bool Graph::reachNodeWithBitset(int nodeOne, int nodeTwo) {
    int nodeOnePostorder = this->getPostOrderOfNode(nodeOne);
    if (NodeIntervalBitsetMap[nodeTwo][nodeOnePostorder]) {
        return true;
    }
}
/*
Checks with a Breadth First Search node One is able to reach node Two
*/
bool Graph::reachNodeBFS(int nodeOne, int nodeTwo) 
{
    queue<int> Q;
    map<int,bool> alreadyVisited;
    Q.push(nodeOne);
    alreadyVisited[nodeOne] == true;
    while (!Q.empty())
    {
        int currnode = Q.front();
        alreadyVisited[currnode] = true;
        Q.pop();
        for(int iter : this->GraphScheme[currnode])
        {
            if (iter == nodeTwo) 
            {
                return true;
            }
            if (!alreadyVisited[iter] && this->GraphScheme.count(currnode) != 0) 
            {
                alreadyVisited[iter] = true;
                Q.push(iter);
            }
        }
    }
    return false;
}

#pragma endregion

#pragma region Create Interval Scheme

void Graph::createPostorderTraversal(string fileName) {
    map<int, bool>* alreadyVisitedPtr = new map<int, bool>;
    postOrder.push_back(minimumSpanningTreeRoot);
    while (!checkIfStackIsFinished(alreadyVisitedPtr))
    {
        postOrder = this->traverseTree(alreadyVisitedPtr);
    }
    
    reverse(postOrder.begin(), postOrder.end());

    for (int i = 0; i < postOrder.size(); i++) {
        postOrderWithIndex[postOrder[i]] = i + 1;
        nodeHasPostorder[i + 1] = postOrder[i];
    }

    ofstream nodeHastPostorderFile("./data/processed/" + fileName + "_postorder");
    if (nodeHastPostorderFile.is_open())
    {
        for (unordered_map<int, int>::iterator it = nodeHasPostorder.begin(); it != nodeHasPostorder.end(); it++)
        {
            nodeHastPostorderFile  << it->second << "\n";
        }
        nodeHastPostorderFile.close();
    }
    cout << "   postorder created! \n";
}

bool Graph::checkIfStackIsFinished(map<int, bool>* alreadyVisitedPointer) 
{
    for (int i : postOrder) 
    {
        if (getNodesMst(i).size() > 0 && !(*alreadyVisitedPointer)[i]) {
            return false;
        }
    }
    return true;
}

vector<int> Graph::traverseTree(map<int,bool>* alreadyVisitedPointer) {
    vector<int> childs;
    vector<int> newStack = postOrder;
    int j = 0;

    for (int i : postOrder) {
        if (!(*alreadyVisitedPointer)[i]) {
            childs = getNodesMst(i);
            newStack.insert(newStack.begin() + j , childs.begin(), childs.end());
            j = j + childs.size();
            (*alreadyVisitedPointer)[i] = true;
        }
        j++;
    }
    return newStack;
}
#pragma endregion

#pragma region Graph Propagation

void Graph::getAllParents(int root, unordered_set<int>* ancestors)
{
    queue<int> Q;
    map<int, bool> alreadyVisited;


    Q.push(root);
    ancestors->insert(root);

    while (!Q.empty())
    {
        int currnode = Q.front();
        if (currnode != root)
        {
            ancestors->insert(currnode);
        }
        Q.pop();
        if (this->AllEdgesGoingIntoKeyNode.count(currnode) != 0)
        {
            ancestors->insert(AllEdgesGoingIntoKeyNode[currnode]->begin(), AllEdgesGoingIntoKeyNode[currnode]->end());
            alreadyVisited[currnode] = true;
        }
        else
        {
            for (int node : this->GraphSchemeReverse[currnode])
            {
                if(!alreadyVisited[node])
                {
                    Q.push(node);
                    alreadyVisited[node] = true;
                }
            }
        }

    }
}

void Graph::getAllChildren(int root, unordered_set<int>* children)
{
    queue<int> Q;
    map<int, bool> alreadyVisited;

    Q.push(root);

    while (!Q.empty())
    {
        int currnode = Q.front();
        children->insert(currnode);
        Q.pop();
        if (this->AllEdgesGoingIntoKeyNode.count(currnode) != 0)
        {
            alreadyVisited[currnode] = true;
            for (unordered_set<int>::iterator t = AllEdgesGoingIntoKeyNode[currnode]->begin(); t != AllEdgesGoingIntoKeyNode[currnode]->end(); t++)
            {
                children->insert(*t);
                alreadyVisited[*t] = true;
            }
        }
        else 
        {
            for (int iterator : this->GraphScheme[currnode])
            {
                if (!alreadyVisited[iterator])
                {
                    
                    Q.push(iterator);
                    alreadyVisited[iterator] = true;

                }
            }
        }
        
    }
}
void getNodesWithZeroIncomingEdges(vector<int>* nodes, unordered_map<int,vector<int>>* currentExpandedGraphReverse, vector<int>* queueNodes){
    for (int i : *nodes){
        // cout << i << "  > " << currentExpandedGraphReverse->count(i) << endl;
        if (currentExpandedGraphReverse->count(i) == 0){
            // cout << "node with 0 incoming edges  " << i << endl;
            queueNodes->push_back(i);
        }
    }
}


void sortByIncomingEdges(vector<int>* nodes, unordered_map<int,vector<int>>* currentExpandedGraphReverse, vector<int>* sortedNodes){
    vector<tuple<int,int>> helperVector;
    for (vector<int>::iterator iter = nodes->begin(); iter != nodes->end(); iter++){
        helperVector.push_back(make_tuple( currentExpandedGraphReverse->operator[](*iter).size(), *iter));
        cout << currentExpandedGraphReverse->operator[](*iter).size() << " ";
    }
    sort(helperVector.begin(), helperVector.end());
    for (int i = 0 ; i < helperVector.size(); i++){
        sortedNodes->push_back(get<1>(helperVector[i]));
    }

    cout << "sorted Nodes: ";
    for (vector<int>::iterator iter = sortedNodes->begin(); iter != sortedNodes->end(); iter++){
        cout << *iter << " ";
    }
    cout << endl;
}

/**
 * @brief Takes address of interval labeling scheme and propagates it to dense intervals
 * 
 * @param IntervalSchemeGraphMap 
 * @param postOrder 
 */
void propagateIntervalLabel(map<int,vector<IntervalScheme>>* IntervalSchemeGraphMap, vector<int>* postOrder){
    map<int,vector<IntervalScheme>>::iterator intervalIterator;
    for (intervalIterator = IntervalSchemeGraphMap->begin(); intervalIterator != IntervalSchemeGraphMap->end(); intervalIterator++){
        vector<IntervalScheme> newInterval;
        vector<IntervalScheme> newCompressedIntervalScheme;
        boost::dynamic_bitset<> IntervalBitsetArray(postOrder->size() + 2);
        for (vector<IntervalScheme>::iterator oldInterval = intervalIterator->second.begin(); oldInterval != intervalIterator->second.end(); oldInterval++)
        {
            IntervalBitsetArray[oldInterval->post] = 1;
        }
        int pre = 0;
        int post = 0;
        for (boost::dynamic_bitset<>::size_type bit = 1; bit < IntervalBitsetArray.size() - 1; bit++) {
            if (IntervalBitsetArray[bit] == 1 && IntervalBitsetArray[(bit - 1)] == 0) {
                pre = bit;
            }
            if (IntervalBitsetArray[bit] == 1 && IntervalBitsetArray[(bit + 1)] == 0) {
                post = bit;
                newInterval.push_back(IntervalScheme(pre, post));
            }
        }
        (*IntervalSchemeGraphMap)[intervalIterator->first] = newInterval;
    }
}

/**
 * @brief Opens file and copies interval labeling scheme of graph into file
 * 
 * @param filepath 
 * @param IntervalSchemeGraphMap 
 */
void writeLabelingSchemeToFile(string filepath, map<int,vector<IntervalScheme>>* IntervalSchemeGraphMap){
    ofstream file;
    file.open(filepath);
    for (map<int,vector<IntervalScheme>>::iterator intervals = IntervalSchemeGraphMap->begin(); intervals != IntervalSchemeGraphMap->end(); intervals++)
    {
        file << intervals->first;
        for (vector<IntervalScheme>::iterator interval = (*IntervalSchemeGraphMap)[intervals->first].begin(); interval != (*IntervalSchemeGraphMap)[intervals->first].end(); interval++){
            file << "\t" << to_string(interval->pre) << "\t" << to_string(interval->post);
        }
        file << endl;
    }
    file.close();
    
}

/**
 * @brief Create a labeling scheme using only one priority queue
 * 
 * @param filepath 
 * @param createReverseScheme 
 * @return double 
 */

double Graph::createLabelingSchemeWithOneQueue(string filepath, bool createReverseScheme){
    this->IntervalSchemeGraphMap.clear();
    // Compare function to evaluate priority queue
    auto compare = [this](int nodeOne, int nodeTwo){
        int degreeNodeOne = GraphScheme.count(nodeOne);
        int degreeNodeTwo = GraphScheme.count(nodeTwo);
        if (degreeNodeOne != degreeNodeTwo)
            return degreeNodeOne > degreeNodeTwo; 
        int postOrderOfNodeOne = getPostOrderOfNode(nodeOne);
        int postOrderOfNodeTwo = getPostOrderOfNode(nodeTwo);
        return postOrderOfNodeOne < postOrderOfNodeTwo;
    };

    Timer clock;
    clock.start();

    // Initialize priority queue
    unordered_set<int> helperSetForTrackingUniqueness;
    priority_queue<int, std::vector<int>, decltype(compare)> queueAccordingToIncomingEdges(compare);
    for (int node  : postOrder){
        queueAccordingToIncomingEdges.push(node);
        helperSetForTrackingUniqueness.insert(node);
    }

    // Create first iteration of appending intervals to every node existing in the spanning tree
    for (auto iter = postOrder.begin(); iter != postOrder.end(); ++iter){
        int postOrderNode = postOrderWithIndex[*iter];
        this->IntervalSchemeGraphMap[*iter] = {IntervalScheme(postOrderNode, postOrderNode)};
    }

    // while loop to progress every entry that lies within the queue
    while(!queueAccordingToIncomingEdges.empty() ){
        int currentNode = queueAccordingToIncomingEdges.top();
        queueAccordingToIncomingEdges.pop();
        helperSetForTrackingUniqueness.erase(currentNode);
        if (createReverseScheme){
            for (int targetNode : GraphSchemeReverse[currentNode]){
                this->IntervalSchemeGraphMap[targetNode].insert(
                    this->IntervalSchemeGraphMap[targetNode].end(),
                    this->IntervalSchemeGraphMap[currentNode].begin(),
                    this->IntervalSchemeGraphMap[currentNode].end()
                );
                if (helperSetForTrackingUniqueness.count(targetNode) == 0){
                    queueAccordingToIncomingEdges.push(targetNode);
                    helperSetForTrackingUniqueness.insert(targetNode);
                }
            }
        } 
            else 
        {
            for (int targetNode : GraphScheme[currentNode]){
                this->IntervalSchemeGraphMap[targetNode].insert(
                    this->IntervalSchemeGraphMap[targetNode].end(),
                    this->IntervalSchemeGraphMap[currentNode].begin(),
                    this->IntervalSchemeGraphMap[currentNode].end()
                );
                if (helperSetForTrackingUniqueness.count(targetNode) == 0){
                    queueAccordingToIncomingEdges.push(targetNode);
                    helperSetForTrackingUniqueness.insert(targetNode);
                }
            }
        }

    }

    // Propagate the labeling scheme
    propagateIntervalLabel(&(this->IntervalSchemeGraphMap), &(this->postOrder));
    double creationTime = clock.stop();
    // Write the labeling scheme to the file
    writeLabelingSchemeToFile(filepath + "_queue", &(this->IntervalSchemeGraphMap));
    writeLabelingSchemeToFile(filepath, &(this->IntervalSchemeGraphMap));
    return creationTime;
}

/**
 * @brief Constructs interval labeling scheme according to algorithm described mention in some paper
 * 
 * @param filepath 
 * @param createReverseScheme 
 * @return double 
 */
double Graph::traditionalGraphPropagation(string filepath, bool createReverseScheme){
    // Clean map 
    this->IntervalSchemeGraphMap.clear();

    // Create copy of spanning tree for tracking of graph propagation progress
    unordered_map<int,vector<int>> currentExpandedGraph = this->GraphSchemeDirectedMST;
    
    Timer clock;
    clock.start();
    // Create inversed version to calculate incoming edges
    unordered_map<int,vector<int>> currentExpandedGraphSchemeReverse;
    for (auto iter : this->GraphSchemeMST){
        for (int targetNode : this->GraphSchemeMST[iter.first]){
            currentExpandedGraphSchemeReverse[targetNode].push_back(iter.first);
        }
    }

    // Compare function to evaluate priority queue
    auto compare = [currentExpandedGraphSchemeReverse ,this](int nodeOne, int nodeTwo){
        int degreeNodeOne = currentExpandedGraphSchemeReverse.count(nodeOne);
        int degreeNodeTwo = currentExpandedGraphSchemeReverse.count(nodeTwo);
        if (degreeNodeOne != degreeNodeTwo)
            return degreeNodeOne > degreeNodeTwo; 

        int postOrderOfNodeOne = getPostOrderOfNode(nodeOne);
        int postOrderOfNodeTwo = getPostOrderOfNode(nodeTwo);
        return postOrderOfNodeOne < postOrderOfNodeTwo;
    };
    
    // Initialize priority queue
    unordered_set<int> helperSetForTrackingUniqueness;
    priority_queue<int, std::vector<int>, decltype(compare)> queueAccordingToIncomingEdges(compare);
    for (int node  : postOrder){
        queueAccordingToIncomingEdges.push(node);
        helperSetForTrackingUniqueness.insert(node);
    }
        


    // Create first iteration of appending intervals to every node existing in the spanning tree
    for (auto iter = postOrder.rbegin(); iter != postOrder.rend(); ++iter){
        int postOrderNode = postOrderWithIndex[*iter];
        this->IntervalSchemeGraphMap[*iter] = {IntervalScheme(postOrderNode, postOrderNode)};
        
    }


    // while loop to progress every entry that lies within the queue
    while(!queueAccordingToIncomingEdges.empty() ){
        int currentNode = queueAccordingToIncomingEdges.top();
        queueAccordingToIncomingEdges.pop();
        for (int targetNode : this->GraphSchemeDirectedMST[currentNode]){
            this->IntervalSchemeGraphMap[targetNode].insert(
                this->IntervalSchemeGraphMap[targetNode].end(),
                this->IntervalSchemeGraphMap[currentNode].begin(),
                this->IntervalSchemeGraphMap[currentNode].end()
            );
            queueAccordingToIncomingEdges.push(targetNode);
        }
    }

    // Reimplemented the non spannin tree edges edges
    for (unordered_map<int,vector<int>>::iterator iter = this->GraphSchemeNonSpanningTreeEdges.begin(); iter != this->GraphSchemeNonSpanningTreeEdges.end(); iter++){
        queueAccordingToIncomingEdges.push(iter->first);
        for(int targetNode : iter->second)
            currentExpandedGraph[iter->first].push_back(targetNode);

        while(!queueAccordingToIncomingEdges.empty() ){
        int currentNode = queueAccordingToIncomingEdges.top();
        queueAccordingToIncomingEdges.pop();
            for (int targetNode : currentExpandedGraph[currentNode]){
                this->IntervalSchemeGraphMap[targetNode].insert(
                    this->IntervalSchemeGraphMap[targetNode].end(),
                    this->IntervalSchemeGraphMap[currentNode].begin(),
                    this->IntervalSchemeGraphMap[currentNode].end()
                );
                queueAccordingToIncomingEdges.push(targetNode);
            }
        }
    }

    this->GraphSchemeDirectedMST.clear();
    this->GraphSchemeNonSpanningTreeEdges.clear();

    propagateIntervalLabel(&(this->IntervalSchemeGraphMap), &(this->postOrder));
    double creationTime = clock.stop();
    // Write the labeling scheme to the file
    writeLabelingSchemeToFile(filepath, &(this->IntervalSchemeGraphMap));
    writeLabelingSchemeToFile(filepath + "_traditional", &(this->IntervalSchemeGraphMap));

    return creationTime;
}


/**
 * @brief Create interval labeling scheme using several or single threads
 * 
 * @param filepath 
 * @param createReverseScheme 
 * @param SpatialGraph 
 * @return double 
 */
double Graph::graphPropagation(string filepath,bool createReverseScheme, LocationMap* SpatialGraph)
{
    Timer clock;
    this->IntervalSchemeGraphMap.clear();
    vector<ofstream*> txtFiles;
    // Get Maximum number of threads defined in main
    int num_of_threads =  omp_get_max_threads();
    
    int chunk_size = ceil(postOrder.size() / num_of_threads);

    clock.start();

    #pragma omp parallel for //schedule(static, chunk_size)
    for(int i = 0; i < postOrder.size(); i++)
    {   
        unordered_set<int> Ancestors;
        createReverseScheme ? getAllChildren(postOrder[i], &Ancestors) : getAllParents(postOrder[i], &Ancestors);
        vector<IntervalScheme> newCompressedIntervalScheme;
        boost::dynamic_bitset<> IntervalBitsetArray(postOrder.size() + 2);
        for (unordered_set<int>::iterator node = Ancestors.begin(); node != Ancestors.end(); node++)
        {
            IntervalBitsetArray[postOrderWithIndex[*node]] = 1;
        }
        int pre = 0;
        int post = 0;
        for (boost::dynamic_bitset<>::size_type bit = 1; bit < IntervalBitsetArray.size() - 1; bit++) {
            if (IntervalBitsetArray[bit] == 1 && IntervalBitsetArray[(bit - 1)] == 0) {
                pre = bit;
            }
            if (IntervalBitsetArray[bit] == 1 && IntervalBitsetArray[(bit + 1)] == 0) {
                post = bit;
                newCompressedIntervalScheme.push_back(IntervalScheme(pre, post));
            }
        }
    
        this->IntervalSchemeGraphMap[postOrder[i]] = newCompressedIntervalScheme;
    }

    double creationTime = clock.stop();
    // Write the labeling scheme to the file
    writeLabelingSchemeToFile(filepath + "_parallel_" + to_string(num_of_threads), &(this->IntervalSchemeGraphMap));
    writeLabelingSchemeToFile(filepath, &(this->IntervalSchemeGraphMap));

    return creationTime;
}

void Graph::topologicalSortUtil(int v, bool visited[], stack<int>& Stack)
{
    visited[v] = true;
    for (int i : GraphScheme[v])
        if (!visited[i])
            topologicalSortUtil(i, visited, Stack);

    Stack.push(v);
}

void Graph::topologicalSort(vector<int>* topologicalSortedVertices)
{
    set<int> VReduced;
    for (unordered_map<int, vector<int>>::iterator node = GraphScheme.begin(); node != GraphScheme.end(); node++) {
        VReduced.insert(node->first);
        for (int entry : node->second) 
            VReduced.insert(entry);
    }

    cout << VReduced.size() << endl;

    stack<int> Stack;
    int size = VReduced.size();
    if (size < *(VReduced.rbegin())) { size = *(VReduced.rbegin()); }
    bool* visited = new bool[size + 1];

    for (int i : VReduced) 
    {
        visited[i] = false;
    }


    for (int i : VReduced) 
    {
        if (visited[i] == false)
        {
            topologicalSortUtil(i, visited, Stack);
        }
    }

    while (Stack.empty() == false) 
    {
        topologicalSortedVertices->push_back(Stack.top());
        Stack.pop();
    }

}
       
#pragma endregion



#pragma region Print Statements

void Graph::print() {
    cout << "Graph Structure:\n";
    unordered_map<int, vector<int>>::iterator node;
    for (node = GraphScheme.begin(); node != GraphScheme.end(); node++) {
        cout << "Node :" << node->first << " -> (";
        for (int entry : node->second) {
            cout << entry << ",";
        }
        cout << ")" << endl;
    }
}

void Graph::printReverse() {
    cout << "print reverse graph structure: " << endl;
    for (unordered_map<int, vector<int>>::iterator node = GraphSchemeReverse.begin(); node != GraphSchemeReverse.end(); node++) {
        cout << "Node :" << node->first << " -> (";
        for (int entry : node->second) {
            cout << entry << ",";
        }
        cout << ")" << endl;
    }
}

void Graph::printPostOrder() {
    cout << "PostOrder Size: " << postOrder.size() << endl;
    cout << " ( ";
    for (int l : postOrder) {
        cout << l << ", ";
    }
    cout << " )" << endl << endl;;
}

void Graph::printIntervalGraphMap() {
    cout << "Interval Graph Map : " << endl;
    for (auto i : IntervalSchemeGraphMap) {
        cout << "Node: " << i.first << " ->  Intervals ";
        for (auto j : IntervalSchemeGraphMap[i.first]) {
            cout << "(" << j.pre << ", " << j.post << "),";

        }
        cout << endl;
    }
    cout << endl;
}

void Graph::printMST() {
    cout << " Print MST : " << endl;
    unordered_map<int, vector<int>>::iterator node;
    for (node = GraphSchemeMST.begin(); node != GraphSchemeMST.end(); node++) {
        if (node->second.size() > 0) 
        {
            cout << "Node :" << node->first << " -> (";
            for (int entry : node->second) {
                cout << entry << ",";
            }
            cout << ")" << endl;
        }

    }
    cout << endl;
}

void Graph::printNode(int node) {
    cout << "Node: " << node << " -> (";
    for (int nbr : GraphScheme[node]) {
        cout << nbr << ",";
    }
    cout << " )" << endl;

}

void Graph::printNodeMst(int node) {
    cout << "NodeMST: " << node << " -> (";
    for (int nbr : GraphSchemeMST[node]) {
        cout << nbr << ",";
    }
    cout << " )" << endl;;
}

void Graph::printSCC() {
    cout << " Print Super Connected Components: " << endl;
    unordered_map<int, vector<int>>::iterator node;
    for (node = SuperConnectedComponents.begin(); node != SuperConnectedComponents.end(); node++) {
        cout << "Supernode :" << node->first << " -> (";
        for (int entry : node->second) {
            cout << entry << ",";
        }
        cout << ")" << endl;
    }
    cout << "All super connected components printed" << endl;
}

void Graph::printMBRWithSCC()
{
    cout << " Print SCC MBR Relation";   
    unordered_map<int, MBR>::iterator iter;
    for (iter = SCCsMBR.begin(); iter != SCCsMBR.end(); iter++)
    {
        cout << (*iter).first << " : " << (*iter).second.getString() << endl;
    }

    cout << "SCC MBR Relationprinted " << endl;
}

void Graph::printSccReverse()
{
    unordered_map<int, int>::iterator it;

    for (it = NodeBelongsToSCC.begin(); it != NodeBelongsToSCC.end(); it++)
    {
        cout << "Node: "<< it->first << " belongs to " << it->second << endl;
    }
}

#pragma endregion

void Graph::writeReducedGraph(string fileName)
{
    cout << "Write reduced graph file..";
    ofstream file;
    file.open("./data/processed/" + fileName + "_reduced_scheme");
    unordered_map<int, vector<int>>::iterator node;
    for (node = GraphScheme.begin(); node != GraphScheme.end(); node++) {
        if (node->second.size() != 0)
        {
            file << to_string(node->first) << "\t";
            for (int entry : node->second) {
                file << to_string(entry) << "\t";
            }
            file << "\n";
        }

    }
    file.close();
    cout << "  finished" << endl;
}

void Graph::readReducedGraph(string filePath)
{
    this->GraphScheme.clear();
    cout << "Read reduced graph file: ";
    ifstream file;
    file.open(filePath);
    string line;
    bool isRootNode = true;
    int rootNode, node, counter;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            stringstream   linestream(line);
            string  nodeString;
            while (getline(linestream, nodeString, '\t')) {
                node = stoi(nodeString);
                if (isRootNode) {
                    rootNode = node;
                    isRootNode = false;
                }
                if (rootNode != node) {
                    counter++;
                    this->GraphScheme[rootNode].push_back(node);
                    this->GraphSchemeReverse[node].push_back(rootNode);
                    this->V.insert(rootNode);
                    this->V.insert(node);

                }
            }
            isRootNode = true;
        }
    }
    cout << counter << " lines read.\n" ;
    file.close();
}

void Graph::writeIntervalSchemeIntoFile(string fileName) 
{
    map <int, vector<IntervalScheme>>::iterator it;
    vector<IntervalScheme>::iterator iterInterval;

    string filePath = ABSOLUTE_FILE_PATH  + "//interval_schemes/" + fileName + "_interval_scheme";
    ofstream myfile(filePath);
    string intervalLine;
    if (myfile.is_open())
    {
        for (it = IntervalSchemeGraphMap.begin(); it != IntervalSchemeGraphMap.end(); it++)
        {
            intervalLine = "";
            intervalLine = intervalLine + to_string(it->first) + '\t';
            for (iterInterval = IntervalSchemeGraphMap[it->first].begin(); iterInterval != IntervalSchemeGraphMap[it->first].end(); iterInterval++) 
            {
                intervalLine = intervalLine + to_string(iterInterval->pre) + '\t' + to_string(iterInterval->post) + '\t';
            }
            myfile << intervalLine << endl;
        }
        myfile.close();
    }
}

void Graph::readIntervalSchemeFromFile(string filePath)
{
    IntervalSchemeGraphMap.clear();
    cout << "Load interval scheme from file... " << endl;
    ifstream file;
    file.open(filePath);
    string line;
    vector<int> IntervalData;
    int node;
    int counter  = 0;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            stringstream   linestream(line);
            string  data;
            while (getline(linestream, data, '\t')) {
                try{

                    IntervalData.push_back(stoi(data));
                    }
                catch (exception& e)
                {
                    //cout << "Standard exception: " << e.what() << endl;
                }
            }
            for (int i = 2; i < IntervalData.size(); i = i+2)
            {
                counter++;
                IntervalSchemeGraphMap[IntervalData[0]].push_back(IntervalScheme(IntervalData[i - 1], IntervalData[i]));
            }

            IntervalData.clear();
        }
    }
        else
    {
        cout << "Interval scheme was not found. PATH : " << filePath << endl;
    }
    file.close();
    cout << counter << " intervals loaded." << endl;
}

vector<int> Graph::getSuperConnectedComponentes(int node) {
    return SuperConnectedComponents[node];
};

void Graph::writeSuperConnectedComponents(string fileName)
{
    cout << "Write super connected components file...";
    unordered_map<int, vector<int>>::iterator it;
    ofstream file;
    file.open("./data/processed/" + fileName +"_strongly_connected_componets");

    for (it = SuperConnectedComponents.begin(); it != SuperConnectedComponents.end(); it++)
    {
        file << to_string(it->first) << "\t";
        for (int node : it->second)
        {
            file << to_string(node) << "\t";
        }
        file << "\n";
    }
    file.close();
    cout << " finished! " << endl;
}

void Graph::readSuperConnectedComponents(string filePath)
{
    cout << "Read strongly connected components" << endl;
    ifstream file;
    file.open(filePath);
    string line;
    int sccNode, node;
    bool sccDoesntExist = true;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            stringstream   linestream(line);
            string  nodeString;
            while (getline(linestream, nodeString, '\t')) {
                node = stoi(nodeString);
                if (sccDoesntExist) {
                    sccNode = node;
                    sccDoesntExist = false;
                }

                if (sccNode != node) {
                    this->SuperConnectedComponents[sccNode].push_back(node);
                    this->NodeBelongsToSCC[node] = sccNode;
                }
            }
            sccDoesntExist = true;
        }
    }
    file.close();
}

void Graph::readPostorder(string filePath) {
    cout << "Read postorder" << endl;
    ifstream file;
    file.open(filePath);
    string line;
    int counter = 1;
    int node;
    this->postOrder.clear();
    this->nodeHasPostorder.clear();

    if (file.is_open()){
        while (file >> node) {
            nodeHasPostorder[counter] = node;
            postOrderWithIndex[node] = counter;
            this->postOrder.push_back(node);
            counter++;
        }
    }
        else
    {
        cout << "postoder file was not found" << endl;
    }
    file.close();
}

vector<int> Graph::DFSUtilReverse(int v, bool visited[], vector<int> nodesInScc)
{
    nodesInScc.push_back(v);
    visited[v] = true;
    cout << v << " ";
    vector<int>::iterator i;
    for (i = GraphSchemeReverse[v].begin(); i != GraphSchemeReverse[v].end(); ++i)
    {
        if (!visited[*i])
        {
            return DFSUtilReverse(*i, visited, nodesInScc);
        }
    }
    return nodesInScc;
}

void Graph::fillOrder(int v, bool visited[], stack<int>& Stack)
{
    // Mark the current node as visited and print it
    visited[v] = true;

    // Recur for all the vertices adjacent to this vertex
    vector<int>::iterator i;
    for (i = GraphScheme[v].begin(); i != GraphScheme[v].end(); ++i)
        if (!visited[*i])
            fillOrder(*i, visited, Stack);

    // All vertices reachable from v are processed by now, push v 
    Stack.push(v);
}

unordered_map<int, vector<int>> cleanDuplicatesFromSCC(unordered_map<int, vector<int>> SCC) {
    unordered_map<int, vector<int>>::iterator it;
    vector<int> nodesToErase;
    for (it = SCC.begin(); it != SCC.end(); it++)
    {
        if (it->second.size() == 1) {
            nodesToErase.push_back(it->first);
        }
    }
    for (int eraseMe : nodesToErase)
    {
        SCC.erase(eraseMe);
    }
    return SCC;
}

void Graph::SCCUtil(int u, int disc[], int low[], stack<int> *st,
                    bool stackMember[])
{
    // A static variable is used for simplicity, we can avoid use
    // of static variable by passing a pointer.
    static int time = 0;
 
    // Initialize discovery time and low value
    cout << "b \n";
    disc[u] = low[u] = ++time;
    st->push(u);
    cout << "t \n";
    stackMember[u] = true;
 
    // Go through all vertices adjacent to this
    for (vector<int>::iterator i = GraphScheme[u].begin(); i != GraphScheme[u].end(); ++i)
    {
        int v = *i; // v is current adjacent of 'u'
        // If v is not visited yet, then recur for it
        cout << "1" << endl;
        if (disc[v] == -1)
        {
            cout << "2 " << v << endl;
            getchar();
            SCCUtil(v, disc, low, st, stackMember);
 
            // Check if the subtree rooted with 'v' has a
            // connection to one of the ancestors of 'u'
            // Case 1 (per above discussion on Disc and Low value)
            cout << "3" << endl;
            low[u] = min(low[u], low[v]);
        }
 
        // Update low value of 'u' only of 'v' is still in stack
        // (i.e. it's a back edge, not cross edge).
        // Case 2 (per above discussion on Disc and Low value)
        else if (stackMember[v] == true){
            cout << "4" << endl;
            low[u] = min(low[u], disc[v]);
        }
    }
 

    // head node found, pop the stack and print an SCC
    int w = 0; // To store stack extracted vertices
    bool isScc = false;
    if (low[u] == disc[u])
    {
        vector<int> sccVector;
        while (st->top() != u)
        {
            if (!isScc)
                this->maximumNode++;
            isScc = true;
            w = (int) st->top();
            this->NodeBelongsToSCC[w] = this->maximumNode;
            sccVector.push_back(w);
            stackMember[w] = false;
            st->pop();
        }
        if (isScc){
            w = (int) st->top();
            sccVector.push_back(w);
            this->NodeBelongsToSCC[w] = this->maximumNode;
            this->V.insert(this->maximumNode);
            this->SuperConnectedComponents[this->maximumNode] = sccVector;
            stackMember[w] = false;
            isScc = false;
        }
       
        st->pop();
    }
}

void Graph::createSCCsUsingTarjans()
{
    cout << "Use Tarjans to create auxiliary structure" << endl;
    int graphSize = V.size();
    if (graphSize < *(V.rbegin()))
        graphSize = *(V.rbegin());
    
    cout << "V size " << V.size() << endl;
    cout << "V end " << *(V.rbegin()) << endl;
    cout << "graphSize " << graphSize << endl;

    int *disc = new int[graphSize];
    int *low = new int[graphSize];
    bool *stackMember = new bool[graphSize];
    stack<int> *st = new stack<int>();
    cout << "im here" << endl;
    // Initialize disc and low, and stackMember arrays
    for (int i : V)
    {
        disc[i] = -1;
        low[i] = -1;
        stackMember[i] = false;
    }
 
    cout << "im here  2" << endl;

    // Call the recursive helper function to find strongly
    // connected components in DFS tree with vertex 'i'
    for (int i : V)
    {
        cout << i << endl;
        if (disc[i] == -1)
        {
            SCCUtil(i, disc, low, st, stackMember);
        }
    }

    cout << "im here  3" << endl;


    /**
     * Check which nodes in the original Scheme belong to strongly connected components
     * Erase those keys and collect the reachable nodes inside the new strongly connected component
     */

    vector<int> keysToErase;
    for (unordered_map<int, vector<int>>::iterator iter =  GraphScheme.begin(); iter != GraphScheme.end(); iter++)
    {
        int currentNode = iter->first;
        vector<int> reachableNodesFromCurrentNode = iter->second;
        if (NodeBelongsToSCC.count(currentNode) > 0){
            int currentStronglyConnectedComponent = NodeBelongsToSCC[currentNode]; 
            keysToErase.push_back(currentNode);
            if (GraphScheme.count(currentStronglyConnectedComponent))
                GraphScheme[currentStronglyConnectedComponent].insert(end(GraphScheme[currentStronglyConnectedComponent]), begin(reachableNodesFromCurrentNode), end(reachableNodesFromCurrentNode));
            else
                GraphScheme[currentStronglyConnectedComponent] = reachableNodesFromCurrentNode;
        }
    }

    cout << "im here  4" << endl;


    /**
     * Erase Keys that are now stored inside strongly connected components
     */

    for (int key : keysToErase){
        GraphScheme.erase(key);
    }


    cout << "im here  5" << endl;

    /**
     * Check which reachable nodes are stored inside a strongly connected component and
     * replace them with the strongly connected component.
     */
    for (unordered_map<int, vector<int>>::iterator iter =  GraphScheme.begin(); iter != GraphScheme.end(); iter++)
    {
        for (int reachableNode : iter->second)
        {
            if (NodeBelongsToSCC.count(reachableNode))
            {
                if (!(find(iter->second.begin(), iter->second.end(),NodeBelongsToSCC[reachableNode] )!=iter->second.end()))
                {
                    if (iter->first != NodeBelongsToSCC[reachableNode])
                    GraphScheme[iter->first].push_back(NodeBelongsToSCC[reachableNode]);
                }
                GraphScheme[iter->first].erase(remove(GraphScheme[iter->first].begin(), GraphScheme[iter->first].end(), reachableNode), GraphScheme[iter->first].end());
            }
        }
    }

    cout << "im here  6" << endl;
   
    /**
     * Clear the orignal Bi and reverse scheme and refill it 
     * with the newly reduced scheme
     */
    this->GraphSchemeReverse.clear();
    this->BiGraphScheme.clear();
    this->BiGraphScheme = this->GraphScheme;
    for (unordered_map<int, vector<int>>::iterator it = GraphScheme.begin(); it != GraphScheme.end(); it++)
    {
        VReduced.insert(it->first);
        for (int i : it->second) {
            VReduced.insert(i);
            this->GraphSchemeReverse[i].push_back(it->first);
            this->BiGraphScheme[i].push_back(it->first);
        }
    }
}

void Graph::createBflFileForQuerying(string filepath){
    
    ofstream file;
    file.open(filepath);
    file << "graph_for_greach \n";
    file << this->VReduced.size() << "\n";

    unordered_map<int,vector<int>>::iterator graphIterator;
    for (graphIterator = this->GraphScheme.begin(); graphIterator != this->GraphScheme.end(); graphIterator++){
        string line = to_string(graphIterator->first) + ":"; 
        for (int targetNode : graphIterator->second){
            line += " " + to_string(targetNode);
        }
        line += "#\n";
        file << line;
    }
}