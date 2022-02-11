import networkx
import random

def construct_random_coordinate(lower_cor, upper_cor):
    return str("{:.5f}".format((random.uniform(float(lower_cor), float(upper_cor)))))


def construct_file_entry(social_node, x, y):  # Construct entry for file
    return (str(social_node) + ',' + str(x) + ',' + str(y) + '\n')


def construct_geo_graph(social_nodes_amount, geo_nodes_amount, bottom_left_coords, top_right_coords, file_name):
    print('Construct spatial graph with {} nodes'.format(geo_nodes_amount))
    geo_file = open("./data/raw/" + file_name + "_spatial", "w")

    for node in range(0, social_nodes_amount):
        if ((random.random()*100) < geo_nodes_amount):
            x = construct_random_coordinate(
                bottom_left_coords[0], top_right_coords[0])
            y = construct_random_coordinate(
                bottom_left_coords[1], top_right_coords[1])
            geo_file.write(construct_file_entry(node, x, y))


def construct_scale_free_graph(social_nodes_amount, file_name):
    print('Construct social graph with {} nodes'.format(social_nodes_amount))
    scale_free_graph = networkx.scale_free_graph(social_nodes_amount, alpha=0.41, beta=0.54,
                                                 gamma=0.05, delta_in=0.2, delta_out=0.2, create_using=networkx.MultiDiGraph, seed=None)
    social_graph_file = open("./data/raw/" + file_name + "_social", "w")
    social_graph_file.truncate(0)
    for edge in scale_free_graph.edges:
        social_graph_file.write(str(edge[0]) + ',' + str(edge[1]) + '\n')


def main():
    print("Specify amount of social nodes:")
    social_nodes = int(input())
    print("Specify percentage of them being spatial (between 0 and 100):")
    spatial_percentage = int(input())

    print("Specify file nname:")
    file_name = input()

    construct_scale_free_graph(social_nodes, file_name)
    construct_geo_graph(social_nodes, spatial_percentage, [0,0], [1,1], file_name)



main()