#ifndef LOCATION_H
#define LOCATION_H

#include<iostream>
#include "../Structures/boostTypes.h"

using namespace std;

struct Location {
    coordinates x;
    coordinates y;

public:
    Location();
    Location(coordinates x, coordinates y);
    void print();
};

#endif