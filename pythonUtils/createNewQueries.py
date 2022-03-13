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

    for line in data:
        new_file.write(
            str(int(line[0])) + "\t" +  str(int(line[1])) + "\t" + str(line[2]) + "\t" + str(line[3]) + "\t" + str(line[4])  + "\t" + str(line[5])  + "\t" + str(line[6]) + "\t" + str(int(line[7])) + "\n"
        )

def findQueriesWithDegreAndArea(file_name, degree=False, area=False, cardinality= False):
    file = open("data/queries/" +
                file_name + "_queries", "r")

    degreeStr = str(degree) if degree else "all"
    areaStr = str(area) if area else "all"
    cardinalityStr = str(cardinality) if cardinality else "all"

    new_file = open("data/queries/" +
                file_name + "_degree_" + degreeStr + "_area_" + areaStr + "_cardinality_" + cardinalityStr, "w")

    next(file)
    data = []
    for line in file:
        line = line.split("\t")

        for i in range(0,len(line)):
            line[i] = float(line[i].replace("\n", ""))
        
        if degree:
            if not (degree - 2 <= int(line[1]) <= degree + 2):
                continue
        if area:
            if float(line[6]) != area:
                continue
        if cardinality:
            if  not (cardinality - (cardinality * 0.1) <= int(line[7]) <= cardinality + (cardinality * 0.1)):
                continue

        data.append(line)

    data.sort(key=lambda row: (row[7]), reverse=True)

    print(len(data))
    if (len(data) > 1000): 
        take_every_n_entry = int(len(data)/ 1000)
        print(take_every_n_entry)
        counter = 0
        for line in data:
            if counter % take_every_n_entry == 0:
                new_file.write(
                    str(int(line[0])) + "\t" +  str(int(line[1])) + "\t" + str(line[2]) + "\t" + str(line[3]) + "\t" + str(line[4])  + "\t" + str(line[5])  + "\t" + str(line[6]) + "\t" + str(int(line[7])) + "\n"
                )
            counter = counter + 1 

    else: 
        for line in data:
            new_file.write(
                str(int(line[0])) + "\t" +  str(int(line[1])) + "\t" + str(line[2]) + "\t" + str(line[3]) + "\t" + str(line[4])  + "\t" + str(line[5])  + "\t" + str(line[6]) + "\t" + str(int(line[7])) + "\n"
            )

    
    

findQueriesWithDegreAndArea("foursquare_all", degree=10, area=0.1)
findQueriesWithDegreAndArea("foursquare_all", cardinality=1000,  area=0.1)
findQueriesWithDegreAndArea("foursquare_all", cardinality=1000, degree=10)

# sortQueries("foursquare")