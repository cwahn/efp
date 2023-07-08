#ifndef TEST_COMMON_HPP_
#define TEST_COMMON_HPP_

#include <array>
#include <vector>

int c_array_3[3] = {1, 2, 3};
int c_array_5[5] = {1, 2, 3, 4, 5};

std::array<double, 3> std_array_3 = {1., 2., 3.};
std::array<double, 5> std_array_5 = {1., 2., 3., 4., 5.};

std::vector<double> std_vector_0 = {};
std::vector<double> std_vector_3 = {1., 2., 3.};
std::vector<double> std_vector_5 = {1., 2., 3., 4., 5.};

#endif