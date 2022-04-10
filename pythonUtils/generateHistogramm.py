import numpy as np
import matplotlib.pyplot as plt
from numpy.lib.shape_base import apply_over_axes


def plot_histogramm_degree(file_name):
    file = open("data/queries/" +
                file_name + "_queries", "r")

    y = []
    next(file)
    for line in file:
        line = line.split("\t")
        y.append(int(line[7]))

    t = []
    # print(y)
    for i in range(1, 20):
        t.append(i)
    print('Durschnitt:', np.average(y))
    old_bins = [1, 2, 4, 8, 16, 32, 64, 128,
                256, 512, 1024, 2048, 5096, 10192]
    plt.hist(y, bins=t)
    plt.xlabel('#Degree')
    plt.ylabel('Amount')
    plt.title('Degree of nodes')
    plt.grid(True)
    plt.show()

def plot_histogramm_cardinality(file_name):
    file = open("data/queries/" +
                file_name + "_queries", "r")

    y = []
    next(file)
    for line in file:
        line = line.split("\t")
        y.append(int(line[7]))

    t = []
    # print(y)

    print('Durschnitt:', np.average(y))
    old_bins = [1, 2, 4, 8, 16, 32, 64, 128,
                256, 512, 1024, 2048, 5096, 10192, 100000]
    plt.hist(y,  density=True)
    plt.xlabel('Cardinality')
    plt.ylabel('Amount')
    plt.title('Cardinality of queries')
    plt.grid(True)
    plt.show()

def plot_hist2d(file_name):
    file = open("data/raw/" +
                file_name + "_spatial", "r")
    x = []
    y = []
    for line in file:
        line = line.split(",")
        if (len(line) == 3):
            if (line[1] != '' and line[2] != ''):
                x.append(float(line[1]))
                y.append(float(line[2]))


    fig, axes = plt.subplots(ncols=1, nrows=1, figsize=(8, 1))
    axes.set_title('Scatterplot')
    axes.plot(x, y, 'ko', color="red", markersize=0.5)

    plt.show()

    plt.hist2d(x, y, bins=30, cmap='Blues')
    cb = plt.colorbar()
    cb.set_label('points in bin')
    plt.show()


plot_hist2d("foursquare_na")

#plot_histogramm_degree("foursquare")
# plot_histogramm_cardinality("foursquare")