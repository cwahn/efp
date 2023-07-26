#ifndef TEST_COMMON_HPP_
#define TEST_COMMON_HPP_

#include <array>
#include <vector>
#include <numeric>

using namespace efp;

Array<double, 0> static_array_0{};
Array<double, 3> static_array_3{1., 2., 3.};
Array<double, 5> static_array_5{1., 2., 3., 4., 5.};

StaticVector<double, 0> static_vector_0{};
StaticVector<double, 3> static_vector_3{1., 2., 3.};
StaticVector<double, 5> static_vector_5{1., 2., 3., 4., 5.};

Vector<double> dynamic_vector_0{};
Vector<double> dynamic_vector_3{1., 2., 3.};
Vector<double> dynamic_vector_5{1., 2., 3., 4., 5.};

int c_array_3[3] = {1, 2, 3};
int c_array_5[5] = {1, 2, 3, 4, 5};

std::array<double, 3> std_array_3{1., 2., 3.};
std::array<double, 5> std_array_5{1., 2., 3., 4., 5.};

std::vector<double> std_vector_0{};
std::vector<double> std_vector_3{1., 2., 3.};
std::vector<double> std_vector_5{1., 2., 3., 4., 5.};

#endif