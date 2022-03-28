import re 

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
    if degree:
        data.sort(key=lambda row: (row[1]), reverse=True)
    if area: 
        data.sort(key=lambda row: (row[6]), reverse=True)
    if cardinality: 
        data.sort(key=lambda row: (row[7]), reverse=True)
    print(len(data))

    amount_of_queries = 10000
    if (len(data) > amount_of_queries): 
        take_every_n_entry = int(len(data)/ amount_of_queries)
        print(take_every_n_entry)
        counter = 0
        for line in data:
            if counter % take_every_n_entry == 0:
                new_file.write(
                    str(int(line[0])) + "\t" +  str(int(line[1])) + "\t" + str(f'{line[2]:f}') + "\t" + str(f'{line[3]:f}') + "\t" + str(f'{line[4]:f}')  + "\t" + str(f'{line[5]:f}')  + "\t" + str(line[6]) + "\t" + str(int(line[7])) + "\n"
                )
            counter = counter + 1 

    else: 
        for line in data:
            new_file.write(
                str(int(line[0])) + "\t" +  str(int(line[1])) + "\t" + str(f'{line[2]:f}') + "\t" + str(f'{line[3]:f}') + "\t" + str(f'{line[4]:f}')  + "\t" + str(f'{line[5]:f}')  + "\t" + str(line[6]) + "\t" + str(int(line[7])) + "\n"
            )

def getAllTrueQueries(query_file, result_file):

    result_file = open("data/results/"  + result_file)
    next(result_file)
    counter_true = 0
    counter_false = 0
    for line in result_file:
        line = line.split(" ")

        if line[1] == "1":
            counter_true = counter_true + 1
        else: 
            counter_false = counter_false + 1
    print(counter_true, " " , counter_false)


print("Enter filename:")
file = input()


# findQueriesWithDegreAndArea(file, cardinality=1000,  area=0.1)
# findQueriesWithDegreAndArea(file, cardinality=1000, degree=10)
findQueriesWithDegreAndArea(file, area=0.1)
findQueriesWithDegreAndArea(file, degree=10)

# getAllTrueQueries("", "foursquare_bfs")

# sortQueries("foursquare")