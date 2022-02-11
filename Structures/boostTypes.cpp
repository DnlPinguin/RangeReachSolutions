#include "boostTypes.h"


void printBox(box area)
{
    cout << "Box: " << area.min_corner().get<0>() << " " << area.min_corner().get<1>() << " " << area.max_corner().get<0>() << " " << area.max_corner().get<1>() << endl;
}