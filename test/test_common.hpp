#ifndef TEST_COMMON_HPP_
#define TEST_COMMON_HPP_

#include <array>
#include <vector>

using namespace efp;

StaticArray<double, 0> static_array_0 {};
StaticArray<double, 3> static_array_3 {1., 2., 3.};
StaticArray<double, 5> static_array_5 {1., 2., 3., 4., 5.};

StaticVector<double, 0> static_vector_0 {};
StaticVector<double, 3> static_vector_3 {1., 2., 3.};
StaticVector<double, 5> static_vector_5 {1., 2., 3., 4., 5.};

DynamicVector<double> dynamic_vector_0 {};
DynamicVector<double> dynamic_vector_3 {1., 2., 3.};
DynamicVector<double> dynamic_vector_5 {1., 2., 3., 4., 5.};

int c_array_3[3] = {1, 2, 3};
int c_array_5[5] = {1, 2, 3, 4, 5};

std::array<double, 3> std_array_3 = {1., 2., 3.};
std::array<double, 5> std_array_5 = {1., 2., 3., 4., 5.};

std::vector<double> std_vector_0 = {};
std::vector<double> std_vector_3 = {1., 2., 3.};
std::vector<double> std_vector_5 = {1., 2., 3., 4., 5.};

#endif