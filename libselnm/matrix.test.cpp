#include <stdexcept>

#include "gtest/gtest.h"
#include "libselnm/matrix.hpp"

namespace {

template <typename T>
class MatrixTest : public testing::Test {};

using MatrixTestTypes = ::testing::Types<float, double, long double>;
TYPED_TEST_SUITE(MatrixTest, MatrixTestTypes);

TYPED_TEST(MatrixTest, InitializeConstant) {
  std::size_t num_rows = 13;
  std::size_t num_cols = 8;
  const TypeParam value = static_cast<TypeParam>(-1.37e-4);
  libselnm::Matrix<TypeParam> m(num_rows, num_cols, value);

  ASSERT_EQ(m.columns(), num_cols);
  ASSERT_EQ(m.rows(), num_rows);
  for (auto i = 0u; i < num_rows; ++i) {
    for (auto j = 0u; j < num_cols; ++j) {
      EXPECT_EQ(m.at(i, j), value);
    }
  }
}

TYPED_TEST(MatrixTest, InitializeList) {
  std::size_t num_rows = 3;
  std::size_t num_cols = 2;
  const TypeParam f1 = static_cast<TypeParam>(1.0);
  const TypeParam f2 = static_cast<TypeParam>(2.0);
  const TypeParam f3 = static_cast<TypeParam>(3.0);
  const TypeParam f4 = static_cast<TypeParam>(4.0);
  const TypeParam f5 = static_cast<TypeParam>(5.0);
  const TypeParam f6 = static_cast<TypeParam>(6.0);
  libselnm::Matrix<TypeParam> m(num_rows, num_cols, {f1, f2, f3, f4, f5, f6});

  ASSERT_EQ(m.columns(), num_cols);
  ASSERT_EQ(m.rows(), num_rows);
  ASSERT_EQ(m[0], f1);
  ASSERT_EQ(m(0, 0), f1);
  ASSERT_EQ(m[1], f2);
  ASSERT_EQ(m(0, 1), f2);
  ASSERT_EQ(m[2], f3);
  ASSERT_EQ(m(1, 0), f3);
  ASSERT_EQ(m[3], f4);
  ASSERT_EQ(m(1, 1), f4);
  ASSERT_EQ(m[4], f5);
  ASSERT_EQ(m(2, 0), f5);
  ASSERT_EQ(m[5], f6);
  ASSERT_EQ(m(2, 1), f6);
}

TYPED_TEST(MatrixTest, Accessors) {
  std::size_t num_rows = 5;
  std::size_t num_cols = 7;
  const TypeParam value0 = static_cast<TypeParam>(0.0);
  const TypeParam value1 = static_cast<TypeParam>(1.0);
  const TypeParam value2 = static_cast<TypeParam>(2.0);
  const TypeParam value3 = static_cast<TypeParam>(3.0);
  const TypeParam value4 = static_cast<TypeParam>(4.0);
  libselnm::Matrix<TypeParam> m(num_rows, num_cols, value0);

  // Check accessors for the sizes
  ASSERT_EQ(m.columns(), num_cols);
  ASSERT_EQ(m.rows(), num_rows);

  for (auto i = 0u; i < num_rows; ++i) {
    for (auto j = 0u; j < num_cols; ++j) {
      // Check "at" accessor with two args
      EXPECT_EQ(m.at(i, j), value0);
      m.at(i, j) = value1;
      EXPECT_EQ(m.at(i, j), value1);

      // Check "at" accessor with single arg
      EXPECT_EQ(m.at(i * num_cols + j), value1);
      m.at(i * num_cols + j) = value2;
      EXPECT_EQ(m.at(i * num_cols + j), value2);

      // Check "[]" accessor
      EXPECT_EQ(m[i * num_cols + j], value2);
      m[i * num_cols + j] = value3;
      EXPECT_EQ(m[i * num_cols + j], value3);

      // Check "()" accessor
      EXPECT_EQ(m(i, j), value3);
      m(i, j) = value4;
      EXPECT_EQ(m(i, j), value4);
    }
  }

  // Check that the "at" accessor does bounds checking
  ASSERT_THROW(m.at(-1, 0), std::out_of_range);
  ASSERT_THROW(m.at(0, -1), std::out_of_range);
  ASSERT_THROW(m.at(-1, 1), std::out_of_range);
  ASSERT_THROW(m.at(1, -1), std::out_of_range);
  ASSERT_THROW(m.at(num_rows, num_cols - 1), std::out_of_range);
  ASSERT_THROW(m.at(num_rows - 1, num_cols), std::out_of_range);
  ASSERT_THROW(m.at(-1), std::out_of_range);
  ASSERT_THROW(m.at(num_rows * num_cols), std::out_of_range);
}

TYPED_TEST(MatrixTest, Equality) {
  std::size_t num_rows = 5;
  std::size_t num_cols = 7;
  const TypeParam value0 = static_cast<TypeParam>(0.0);
  const TypeParam value1 = static_cast<TypeParam>(1.0);
  libselnm::Matrix<TypeParam> m(num_rows, num_cols, value0);
  libselnm::Matrix<TypeParam> n(num_rows, num_cols, value0);
  libselnm::Matrix<TypeParam> o(num_rows + 1, num_cols, value0);
  libselnm::Matrix<TypeParam> p(num_rows, num_cols + 1, value0);
  libselnm::Matrix<TypeParam> q(num_rows, num_cols, value0);
  q(0, 0) = value1;

  // Check equality
  EXPECT_TRUE(equal(m, n));
  EXPECT_TRUE(m == n);
  EXPECT_FALSE(m != n);

  // Check inequality due to different number of rows
  EXPECT_FALSE(equal(m, o));
  EXPECT_FALSE(m == o);
  EXPECT_TRUE(m != o);

  // Check inequality due to different number of columns
  EXPECT_FALSE(equal(m, p));
  EXPECT_FALSE(m == p);
  EXPECT_TRUE(m != p);

  // Check inequality due to different values
  EXPECT_TRUE(equal(m, q, value1 - value0));
  EXPECT_FALSE(equal(m, q));
  EXPECT_FALSE(m == q);
  EXPECT_TRUE(m != q);
}

TYPED_TEST(MatrixTest, ScalarAddition) {
  std::size_t num_rows = 5;
  std::size_t num_cols = 7;
  const TypeParam value0 = static_cast<TypeParam>(2.0);
  const TypeParam value1 = static_cast<TypeParam>(4.0);
  libselnm::Matrix<TypeParam> m(num_rows, num_cols, value0);
  libselnm::Matrix<TypeParam> n(num_rows, num_cols, value0 + value1);
  libselnm::Matrix<TypeParam> o(num_rows, num_cols, value0);

  // Check compound assignment
  EXPECT_EQ(m += value1, n);

  // Check matrix + scalar
  libselnm::Matrix<TypeParam> p = o + value1;
  EXPECT_EQ(p, n);
}

TYPED_TEST(MatrixTest, ScalarSubtraction) {
  std::size_t num_rows = 5;
  std::size_t num_cols = 7;
  const TypeParam value0 = static_cast<TypeParam>(2.0);
  const TypeParam value1 = static_cast<TypeParam>(4.0);
  libselnm::Matrix<TypeParam> m(num_rows, num_cols, value0);
  libselnm::Matrix<TypeParam> n(num_rows, num_cols, value0 - value1);
  libselnm::Matrix<TypeParam> o(num_rows, num_cols, value0);

  // Check compound assignment
  EXPECT_EQ(m -= value1, n);

  // Check matrix - scalar
  libselnm::Matrix<TypeParam> p = o - value1;
  EXPECT_EQ(p, n);
}

TYPED_TEST(MatrixTest, ScalarMultiplication) {
  std::size_t num_rows = 5;
  std::size_t num_cols = 7;
  const TypeParam value0 = static_cast<TypeParam>(2.0);
  const TypeParam value1 = static_cast<TypeParam>(4.0);
  libselnm::Matrix<TypeParam> m(num_rows, num_cols, value0);
  libselnm::Matrix<TypeParam> n(num_rows, num_cols, value0 * value1);
  libselnm::Matrix<TypeParam> o(num_rows, num_cols, value0);

  // Check compound assignment
  EXPECT_EQ(m *= value1, n);

  // Check matrix * scalar
  libselnm::Matrix<TypeParam> p = o * value1;
  EXPECT_EQ(p, n);
}

TYPED_TEST(MatrixTest, ScalarDivision) {
  std::size_t num_rows = 5;
  std::size_t num_cols = 7;
  const TypeParam value0 = static_cast<TypeParam>(2.0);
  const TypeParam value1 = static_cast<TypeParam>(4.0);
  libselnm::Matrix<TypeParam> m(num_rows, num_cols, value0);
  libselnm::Matrix<TypeParam> n(num_rows, num_cols, value0 / value1);
  libselnm::Matrix<TypeParam> o(num_rows, num_cols, value0);

  // Check compound assignment
  EXPECT_EQ(m /= value1, n);

  // Check matrix / scalar
  libselnm::Matrix<TypeParam> p = o / value1;
  EXPECT_EQ(p, n);
}

TYPED_TEST(MatrixTest, MatrixAddition) {
  std::size_t num_rows = 5;
  std::size_t num_cols = 7;
  const TypeParam value0 = static_cast<TypeParam>(2.0);
  const TypeParam value1 = static_cast<TypeParam>(4.0);
  libselnm::Matrix<TypeParam> m(num_rows, num_cols, value0);
  libselnm::Matrix<TypeParam> mm(num_rows, num_cols, value0);
  libselnm::Matrix<TypeParam> n(num_rows, num_cols, value1);
  libselnm::Matrix<TypeParam> o(num_rows, num_cols, value0 + value1);

  // Check compound assignment
  EXPECT_EQ(m += n, o);

  // Check matrix + matrix
  libselnm::Matrix<TypeParam> p = mm + n;
  EXPECT_EQ(p, o);

  // Check exception on incompatible sizes
  libselnm::Matrix<TypeParam> a(5, 5, 0.0);
  libselnm::Matrix<TypeParam> b(6, 5, 0.0);
  libselnm::Matrix<TypeParam> c(5, 6, 0.0);
  EXPECT_THROW(a + b, std::invalid_argument);
  EXPECT_THROW(a + c, std::invalid_argument);
}

TYPED_TEST(MatrixTest, MatrixMultiplication) {
  std::size_t m = 2;
  std::size_t n = 3;
  libselnm::Matrix<TypeParam> A(m, n, {1, 2, 3, 4, 5, 6});
  libselnm::Matrix<TypeParam> B(n, m, {1, 2, 3, 4, 5, 6});
  libselnm::Matrix<TypeParam> C(m, m, {22, 28, 49, 64});
  libselnm::Matrix<TypeParam> D(n, n, {9, 12, 15, 19, 26, 33, 29, 40, 51});

  // Check matrix * matrix
  EXPECT_EQ(A * B, C);
  EXPECT_EQ(B * A, D);

  // Check exception on incompatible sizes
  EXPECT_THROW(A * C, std::invalid_argument);
  EXPECT_THROW(B * D, std::invalid_argument);
}

}  // namespace