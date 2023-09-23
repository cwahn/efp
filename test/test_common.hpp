#ifndef TEST_COMMON_HPP_
#define TEST_COMMON_HPP_

#include <array>
#include <vector>
#include <numeric>

using namespace efp;

const double c_array_3[3] = {1., 2., 3.};
const double c_array_5[5] = {1., 2., 3., 4., 5.};

const std::array<double, 0> std_array_0{};
const std::array<double, 3> std_array_3{1., 2., 3.};
const std::array<double, 5> std_array_5{1., 2., 3., 4., 5.};

const std::vector<double> std_vector_0{};
const std::vector<double> std_vector_3{1., 2., 3.};
const std::vector<double> std_vector_5{1., 2., 3., 4., 5.};

// const Array<double, 0> array_0{};
// const Array<double, 3> array_3{1., 2., 3.};
// const Array<double, 5> array_5{1., 2., 3., 4., 5.};

// const ArrVec<double, 0> array_vector_0{};
// const ArrVec<double, 3> array_vector_3{1., 2., 3.};
// const ArrVec<double, 5> array_vector_5{1., 2., 3., 4., 5.};

// const Vector<double> vector_0{};
// const Vector<double> vector_3{1., 2., 3.};
// const Vector<double> vector_5{1., 2., 3., 4., 5.};

// const ArrayView<const double, std_array_0.size()> array_view_0{std_array_0.data()};
// const ArrayView<const double, 3> array_view_3{&c_array_3[0]};
// const ArrayView<const double, array_5.size()> array_view_5{array_5.data()};

// const VectorView<const double> vector_view_0{std_array_0.data(), 0};
// const VectorView<const double> vector_view_3{&c_array_3[0], 3};
// const VectorView<const double> vector_view_5{array_5.data(), array_5.size()};

#endif