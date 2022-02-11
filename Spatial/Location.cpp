#include "location.h"


Location::Location() {
    this->x = 0;
    this->y = 0;
}
Location::Location(coordinates x, coordinates y) {
    this->x = x;
    this->y = y;
}

void Location::print() {
    cout << "Location: xCor -> " << x << "  | yCor -> " << y << endl;;
}
