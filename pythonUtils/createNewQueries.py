def sortQueries(file_name):
    file = open("data/queries/" +
                file_name + "_queries", "r")

    new_file = open("data/queries/" +
                file_name + "_cardiality_asc_queries", "w")

    next(file)
    data = []
    for line in file:
        line = line.split("\t")

        for i in range(0,len(line)):
            line[i] = float(line[i].replace("\n", ""))
        
        data.append(line)


    data.sort(key=lambda row: (row[7]), reverse=True)

    ne
    for line in data:
        new_file.write(
            str(int(line[0])) + "\t" +  str(int(line[1])) + "\t" + str(line[2]) + "\t" + str(line[3]) + "\t" + str(line[4])  + "\t" + str(line[5])  + "\t" + str(line[6]) + "\t" + str(int(line[7])) + "\n"
        )


sortQueries("foursquare")