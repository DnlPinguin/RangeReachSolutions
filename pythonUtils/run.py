import postorder
import scc

print("Specify file name:")

file = input()

scc.construct_super_nodes_on_graph(file)
postorder.construct_postorder_on_nodes(file)