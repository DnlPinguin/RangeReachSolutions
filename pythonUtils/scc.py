import networkx
import numpy 
import MinimumBoundingRectangle
import time

def create_greateast_weakyl_connected_component(file_name):
    G = networkx.DiGraph()

    graph_file = open(
        "./data/raw/{}_social".format(file_name), "r")

    counter = 0
    for edge in graph_file:
        edge = edge.replace("\n", "").split(",")
        G.add_edge(int(edge[0]), int(edge[1]))
        counter = counter + 1
        if (counter % 3000000 == 0):
            print(counter, "edges read")

    print("Graph read")
    wcc = networkx.weakly_connected_components(G)
    print("WCCs created")
    biggest_wcc = []

    for connected_component in wcc:
        connected_component = list(connected_component)
        if (len(connected_component) > len(biggest_wcc)):
            biggest_wcc = connected_component

    print("biggest wcc found")
    biggest_wcc = set(biggest_wcc)
    graph_file_wcc = open(
        "./data/raw/{}_wcc_social".format(file_name), "w")
    
    for edge in G.edges:
        if (edge[0] in biggest_wcc and edge[1] in biggest_wcc):
            graph_file_wcc.write(
                str(edge[0]) + "," + str(edge[1]) + "\n"
            ) 

    print("wcc graph file written")

    spatial_file_wcc = open(
        "./data/raw/{}_wcc_spatial".format(file_name), "w")
    
    spatial_file = open(
        "./data/raw/{}_spatial".format(file_name), "r")


    counter = 0
    for point in spatial_file:
        point = point.replace("\n", "").split(",")
        if (int(point[0]) in biggest_wcc):
            spatial_file_wcc.write(str(point[0]) + "," + str(point[1]) + "," + str(point[2]) + "\n")
        counter = counter + 1
        if (counter % 500000 == 0):
            print(counter, " spatial points transformed")
    print("finished")

def check_dag(file_name):
    G_G = networkx.DiGraph()

    graph_file_one = open(
        "./data/raw/{}_social".format(file_name), "r")

    counter = 0
    for edge in graph_file_one:
        edge = edge.replace("\n", "").split(",")
        G_G.add_edge(int(edge[0]), int(edge[1]))
        counter = counter + 1
        if (counter % 3000000 == 0):
            print(counter, "edges read")

    print("Graph read")

    # print("Is Graph strongly connected ", networkx.is_strongly_connected(G_G))
    # print("Is Graph weakly connected ", networkx.is_weakly_connected(G_G))
    wcc = networkx.weakly_connected_components(G_G)
    for super_connected_component in wcc:
        super_connected_component = list(super_connected_component)
        print(len(super_connected_component), end=" ")

        # max_node = max_node + 1
        # scc_dict[max_node] = super_connected_component
        # scc_file.write(str(max_node))
        # for i in super_connected_component:
        #     scc_file.write("\t" + str(i))
        #         node_belongs_to_scc[i] = max_node
        #     scc_file.write("\n")
        # del(super_connected_component)
    print()
    print("Number of weakly connected components ", networkx.number_weakly_connected_components(G_G))
    print("Number of strongly connected components: ", networkx.number_strongly_connected_components(G_G))
    print("Number of nodes: ", len(G_G))
    print("Is Graph Dag ", networkx.is_directed_acyclic_graph(G_G))

# Build Super Connected Components on existing graph.
def construct_super_nodes_on_graph(file_name):

    G = networkx.DiGraph()
    Reduced_G = networkx.DiGraph()
    
    graph_file = open(
        "./data/raw/{}_social".format(file_name), "r")

    start = int(time.time())
    # read graph file
    max_node = -1
    counter = 0
    for edge in graph_file:
        edge = edge.replace("\n", "").split(",")
        max_node = max(max_node, int(edge[0]), int(edge[1]))
        G.add_edge(int(edge[0]), int(edge[1]))
        counter = counter + 1
        if (counter % 3000000 == 0):
            print(counter, "edges read")
    graph_file.close()
    print("Graph copied into memory: ", int(time.time()) - start, " sec")

    start = int(time.time())
    scc = networkx.kosaraju_strongly_connected_components(G)
    print("Scc construction: ", int(time.time()) - start, " sec")

    print("Number of nodes: ", len(G))
    print("Number of Sccs: ", networkx.number_strongly_connected_components(G))
    scc_dict = {}
    node_belongs_to_scc = {}

    scc_file = open(
        "./data/processed/{}_strongly_connected_components".format(file_name), "w")
    counter = 0

    start = int(time.time())
    for super_connected_component in scc:
        super_connected_component = list(super_connected_component)
        if (len(super_connected_component) > 1):
            max_node = max_node + 1
            scc_dict[max_node] = super_connected_component
            scc_file.write(str(max_node))
            for i in super_connected_component:
                scc_file.write("\t" + str(i))
                node_belongs_to_scc[i] = max_node
            scc_file.write("\n")
        del(super_connected_component)

    print("Copy scc to file: ", int(time.time()) - start, " sec")

    scc_file.close()
    del(scc)
    start = int(time.time())
    #reduce graph to reduced scheme
    counter = 0    
    reduced_graph = {}
    for edge in G.edges:
        edge = list(edge)
        if (edge[0] in node_belongs_to_scc):
            edge[0] = node_belongs_to_scc[edge[0]]
        if (edge[1] in node_belongs_to_scc):
            edge[1] = node_belongs_to_scc[edge[1]]
        if (edge[0] != edge[1]):
            if (edge[0] in reduced_graph):
                reduced_graph[edge[0]].append(edge[1])
            else:
                reduced_graph[edge[0]] = [edge[1]]
        counter = counter + 1
    del G
    
    print("Reduced scheme construction: " , int(time.time()) - start, " sec")
   
    #write reduced scheme
    reduced_scheme_file = open(
        "./data/processed/{}_reduced_scheme".format(file_name), "w")

    start = int(time.time())
    for key in reduced_graph:
        unique_list = numpy.unique(numpy.array(reduced_graph[key]))
        line = str(key)
        for i in unique_list:
            line += "\t" + str(i)
        line += "\n"
        reduced_scheme_file.write(line)
        
    print("Copy reduced scheme to file: ", int(time.time()) - start, " sec" )
    del reduced_graph

     # read spatial file
    
    spatial_file = open(
        "./data/raw/{}_spatial".format(file_name), "r")
    spatial_dict = {}
    for edge in spatial_file:
        edge = edge.replace("\n", "").split(",")
        if (edge[1] != '' and edge[2] != ''):
            spatial_dict[int(edge[0])] = [float(edge[1]), float(edge[2])]

    # create compressed spatial file
    compressed_spatial_file = open(
        "./data/processed/{}_reduced_spatial_data".format(file_name), "w"
    )
    start = int(time.time())
    compressed_spatial_dict = {}
    for spatial_node in spatial_dict:
        
        if (spatial_node in node_belongs_to_scc):
            scc_node = node_belongs_to_scc[spatial_node]
            if (scc_node in compressed_spatial_dict):
                compressed_spatial_dict[scc_node].append(spatial_dict[spatial_node][0], spatial_dict[spatial_node][1])
            else:
                compressed_spatial_dict[scc_node] = MinimumBoundingRectangle.MinimumBoundingRectangle(spatial_dict[spatial_node][0], spatial_dict[spatial_node][1])
        else:
            compressed_spatial_dict[spatial_node] =MinimumBoundingRectangle.MinimumBoundingRectangle(spatial_dict[spatial_node][0], spatial_dict[spatial_node][1])
    for i in compressed_spatial_dict:
        compressed_spatial_file.write(compressed_spatial_dict[i].createLineFormat(i))

    print("Compressed Spatial file created: ", int(time.time()) - start, " sec")
    return 

    

   
    


