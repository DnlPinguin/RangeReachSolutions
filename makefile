OS := $(shell uname)
ifeq ($(OS),Darwin)
        #CC      = /usr/local/opt/llvm/bin/clang++
        #CFLAGS  = -O3 -mavx -std=c++14 -w -march=native -I/usr/local/opt/llvm/include -fopenmp
        #LDFLAGS = -L/usr/local/opt/llvm/lib
	CC	= /usr/local/bin/g++-ls
        CFLAGS  = -O3 -mavx -std=c++17 -w -march=native -fopenmp
        LDFLAGS =
else
        CC      = g++
        CFLAGS  = -O3 -mavx -std=c++17 -w -march=native -fopenmp
        LDFLAGS =
endif

SOURCES = Structures/IntervalScheme.cpp Spatial/Location.cpp Spatial/LocationMap.cpp Spatial/MBR.cpp Queries/helper.cpp Queries/hybridQueries.cpp Queries/socialQueries.cpp Queries/spatialQueries.cpp GeoReach/RangeReach.cpp FileOperations/fileLoading.cpp Graph/graph.cpp Spatial/rTree.cpp Helper/clock.cpp Helper/utility.cpp Structures/boostTypes.cpp bfl/bfl.cpp 
OBJECTS = $(SOURCES:.cpp=.o)

all:  createIntervalScheme createHybrid createQueries createRangeReach createSocialFirst createSpatialFirst createTransitiveClosure createBflStructure runBreadthFirstQuery runHybrid runQueries runRangeReach runSocialFirst runSpatialFirst

createIntervalScheme: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createIntervalScheme.cpp -o Executables/createIntervalScheme $(LDADD)

runQueries: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) runQueries.cpp -o Executables/runQueries $(LDADD)

runSpatialFirst: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) runSpatialFirst.cpp -o Executables/runSpatialFirst $(LDADD)

runSocialFirst: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) runSocialFirst.cpp -o Executables/runSocialFirst $(LDADD)

runBreadthFirstQuery: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) runBreadthFirstQuery.cpp -o Executables/runBreadthFirstQuery $(LDADD)

runHybrid: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) runHybrid.cpp -o Executables/runHybrid $(LDADD)

runRangeReach: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) runRangeReach.cpp -o Executables/runRangeReach $(LDADD)

createQueries: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createQueries.cpp -o Executables/createQueries $(LDADD)

createHybrid : $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createHybrid.cpp -o Executables/createHybrid $(LDADD)

createRangeReach: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createRangeReach.cpp -o Executables/createRangeReach $(LDADD)

createSocialFirst : $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createSocialFirst.cpp -o Executables/createSocialFirst $(LDADD)

createSpatialFirst: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createSpatialFirst.cpp -o Executables/createSpatialFirst $(LDADD)

createTransitiveClosure: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createTransitiveClosure.cpp -o Executables/createTransitiveClosure $(LDADD)

createBflStructure: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createBflStructure.cpp -o Executables/createBflStructure $(LDADD)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cc.o:
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf *.o
	rm -rf run
	#rm -rf create
