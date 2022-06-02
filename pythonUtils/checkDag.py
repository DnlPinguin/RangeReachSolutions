import networkx

print("Specify file name:")

file_name = input()

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

print("Is Graph strongly connected ", networkx.is_strongly_connected(G_G))
print("Is Graph weakly connected ", networkx.is_weakly_connected(G_G))
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

