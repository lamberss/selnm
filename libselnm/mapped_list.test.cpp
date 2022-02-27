#include <sstream>
#include <stdexcept>

#include "gtest/gtest.h"
#include "libselnm/mapped_list.hpp"

namespace {

template <typename T>
class MappedListTest : public testing::Test {};

using MappedListTestTypes = ::testing::Types<float, double, long double>;
TYPED_TEST_SUITE(MappedListTest, MappedListTestTypes);

TYPED_TEST(MappedListTest, InitializeConstant) {
  libselnm::MappedList<TypeParam> list("test_list", "Test Stuff");
  EXPECT_EQ(list.description(), "Test Stuff");
  EXPECT_EQ(list.name(), "test_list");
  EXPECT_EQ(list.size(), 0);
}

TYPED_TEST(MappedListTest, AddItem) {
  libselnm::MappedList<TypeParam> list("test_list", "Test Stuff");

  // Check that adding things increases the size
  list.add("first thing", 0.0);
  list.add("second thing", 1.0);
  EXPECT_EQ(list.size(), 2);

  // Check that adding something that has already been added fails
  EXPECT_THROW(list.add("first thing", 2.0), std::runtime_error);
  EXPECT_THROW(list.add("second thing", 2.0), std::runtime_error);
}

TYPED_TEST(MappedListTest, AccessItem) {
  libselnm::MappedList<TypeParam> list("test_list", "Test Stuff");
  list.add("first thing", 0.0);
  list.add("second thing", 1.0);

  // Check access by index
  EXPECT_EQ(list[0], 0.0);
  EXPECT_EQ(list[1], 1.0);
  EXPECT_EQ(list.at(0), 0.0);
  EXPECT_EQ(list.at(1), 1.0);

  // Check access by ID
  EXPECT_EQ(list.at("first thing"), 0.0);
  EXPECT_EQ(list.at("second thing"), 1.0);

  // Check invalid access
  EXPECT_THROW(list["third thing"], std::out_of_range);
  EXPECT_THROW(list.at("third thing"), std::out_of_range);
  EXPECT_THROW(list.at(2), std::out_of_range);

  // Allow operator[] for indices to go out of bounds so it can iterate quickly
  EXPECT_NO_THROW(list[2]);
}

TYPED_TEST(MappedListTest, StreamInsertion) {
  libselnm::MappedList<TypeParam> list("test_list", "Test Stuff");
  list.add("first thing", 0.0);
  list.add("second thing", 1.0);
  std::stringstream expected;
  expected << "Index            ID  Test Stuff" << std::endl;
  expected << "    0   first thing  0" << std::endl;
  expected << "    1  second thing  1" << std::endl;

  // Check output stream insertion
  std::stringstream actual;
  actual << list;
  EXPECT_EQ(actual.str(), expected.str());
}

}  // namespace