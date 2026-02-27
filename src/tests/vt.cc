#include <gtest/gtest.h>
#include "../vt/vt.h"

vt::vector<int> array = {5, 7, -3, 0, 1};

TEST(VtIntVector, BasicElementAccess) {
  EXPECT_EQ(array[0], 5);
  EXPECT_EQ(array[1], 7);
  EXPECT_EQ(array[4], 1);

  EXPECT_EQ(array.at(0), 5);
  EXPECT_EQ(array.at(2), -3);
  EXPECT_EQ(array.at(4), 1);

  EXPECT_EQ(array.front(), 5);
  EXPECT_EQ(array.back(), 1);
}

TEST(VtIntVector, BasicCapacity) {
  EXPECT_FALSE(array.empty());

  EXPECT_EQ(array.size(), 5);
}

TEST(VtIntVector, BasicModifiers) {
  array.pop_back();

  EXPECT_EQ(array.size(), 4);

  int item = -17;
  array.push_back(item);

  EXPECT_EQ(array.back(), item);
  EXPECT_EQ(array.size(), 5);

  item = 3;
  array.push_back(item);
  array.push_back(item);
  array.push_back(item);
  array.push_back(item);

  EXPECT_EQ(array.back(), item);
  EXPECT_EQ(array.size(), 9);
}