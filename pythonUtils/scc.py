import networkx
import numpy 
import MinimumBoundingRectangle

# Build Super Connected Components on existing graph.
def construct_super_nodes_on_graph(file_name):

    G = networkx.DiGraph()
    Reduced_G = networkx.DiGraph()
    
    graph_file = open(
        "./data/raw/{}_social".format(file_name), "r")

    # read graph file
    max_node = -1
    counter = 0
    for edge in graph_file:
        edge = edge.replace("\n", "").split(",")
        max_node = max(max_node, int(edge[0]), int(edge[1]))
        G.add_edge(int(edge[0]), int(edge[1]))
        counter = counter + 1
        if (counter % 1000000 == 0):
            print(counter, "edges read")
    print("graph read")
    graph_file.close()

    scc = networkx.strongly_connected_components(G)
    print("scc constructed")
    scc_dict = {}
    node_belongs_to_scc = {}
    scc_file = open(
        "./data/processed/{}_strongly_connected_components".format(file_name), "w")
        
    for super_connected_component in scc:
        super_connected_component = list(super_connected_component)
        if (len(super_connected_component) > 1):
            max_node = max_node + 1
            scc_dict[max_node] = super_connected_component
            line = str(max_node)
            for i in super_connected_component:
                line += "\t" + str(i)
                node_belongs_to_scc[i] = max_node
            line += "\n"
            scc_file.write(line)

    scc_file.close()
    del(super_connected_component)
    print("scc dicts created")

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
    
    print("reduced_created")
    #write reduced scheme
    reduced_scheme_file = open(
        "./data/processed/{}_reduced_scheme".format(file_name), "w")

    for key in reduced_graph:
        unique_list = numpy.unique(numpy.array(reduced_graph[key]))
        line = str(key)
        for i in unique_list:
            line += "\t" + str(i)
        line += "\n"
        reduced_scheme_file.write(line)
        
    print("reduced written")
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

    print("compressed spatial created")
    return 

    

   
    


