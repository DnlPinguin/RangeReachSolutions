#ifndef MBR_H
#define MBR_H

#include <string>
#include "location.h"
#include "../Structures/boostTypes.h"
using namespace std;

struct MBR {
	

public:
	coordinates xMin;
	coordinates xMax;
	coordinates yMin;
	coordinates yMax;
	MBR();
	// Creates a Minimum Bounding Reactangle according to the parameter;
	MBR(Location loc);

	MBR(coordinates _xMin, coordinates _yMin, coordinates _xMax, coordinates _yMax);

	//Expands the Miniumum Bound Reactangle according to the parameter;
	void insertLoc(Location loc);
	void insertMBR(MBR mbr);
	bool containsPoint(Location loc);
	
	void print();
	string getString();
	string stringify();
	//Checks if the MBR overlaps with the seconds MBR
	bool overlaps(MBR other);
	bool atleastThreeCornersInside(MBR other);

};

#endif