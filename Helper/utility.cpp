#include "utility.h"


bool in_array(std::string &value, std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}


std::vector<float> createLinspace(float start_in, float end_in, int num_in)
{
  std::vector<float> linspaced;

  float length = end_in - start_in;
  float steps = length / num_in;

  for (float i = 0; i < num_in; i++)
  {
    linspaced.push_back(start_in + steps * i);    
  }
  return linspaced;
}