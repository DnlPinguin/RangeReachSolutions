#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <algorithm>

bool in_array(std::string &value,std::vector<std::string> &array);

std::vector<float> createLinspace(float start_in, float end_in, int num_in);

#endif