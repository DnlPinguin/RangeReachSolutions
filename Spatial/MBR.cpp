
#include "MBR.h"


MBR::MBR() {
	xMax = INT_MIN;
	xMin = INT_MAX;
	yMax = INT_MIN;
	yMin = INT_MAX;
}
MBR::MBR(Location loc) {
	xMax = loc.x;
	xMin = loc.x;
	yMax = loc.y;
	yMin = loc.y;
}

MBR::MBR(coordinates _xMin, coordinates _yMin, coordinates _xMax, coordinates _yMax) {
	xMax = _xMax;
	xMin = _xMin;
	yMax = _yMax;
	yMin = _yMin;
}

void MBR::insertMBR(MBR mbr){
	if (mbr.xMax > xMax) { xMax = mbr.xMax; }
	if (mbr.xMin < xMin) { xMin = mbr.xMin; }
	if (mbr.yMax > yMax) { yMax = mbr.yMax; }
	if (mbr.yMin < yMin) { yMin = mbr.yMin; }
}

void MBR::insertLoc(Location loc) {
	if (loc.x > xMax) { xMax = loc.x; }
	if (loc.x < xMin) { xMin = loc.x; }
	if (loc.y > yMax) { yMax = loc.y; }
	if (loc.y < yMin) { yMin = loc.y; }
}

string MBR::getString() {
	string output = "MBR (xMin, yMin, xMax, yMax)  |  (" + to_string(xMin)  + ", " + to_string(yMin) + ", " + to_string(xMax) + ", " + to_string(yMax) + " ) ";
	return output;
}

string MBR::stringify() {
	return to_string(xMin) + "\t" + to_string(yMin) + "\t"  + to_string(xMax) +"\t"  + to_string(yMax);
}

bool MBR::overlaps(MBR other) {
	if (this->xMin <= other.xMin && this->xMax >= other.xMax && this->yMin <= other.yMin && this->yMax >= other.yMax) {
		return true;
	}
	return false;
}

bool MBR::atleastThreeCornersInside(MBR other){
	int cornersEqual = 0;
	if (this->xMin >= other.xMin)
		cornersEqual++;
	if (this->yMin >= other.yMin)
		cornersEqual++;
	if (this->xMax <= other.xMax)
		cornersEqual++;
	if (this->yMax <= other.yMax)
		cornersEqual++;
	if (cornersEqual > 2)
		return true;
	return false;
}

void MBR::print() {
	cout << this->getString() << endl;
}
;
