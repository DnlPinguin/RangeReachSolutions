#ifndef INTERVALSCHEME_H
#define INTERVALSCHEME_H

// Object for storing Interval Scheme for Social Nodes

#include<string>
struct IntervalScheme {
    int pre;
    int post;

public:
    IntervalScheme();
    IntervalScheme(int pre, int post);
    void print();
};

#endif INTERVALSCHEME;