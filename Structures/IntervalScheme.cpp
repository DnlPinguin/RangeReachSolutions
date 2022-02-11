#include<iostream>
#include "IntervalScheme.h"

using namespace std;


IntervalScheme::IntervalScheme() {

}
IntervalScheme::IntervalScheme(int pre, int post) {
    this->post = post;
    this->pre = pre;
}

void IntervalScheme::print() {
    cout << "Interval (" << pre << " , " << post << " )" << endl;;
}

