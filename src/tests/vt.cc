#include "../vt/vt.hpp"

#include <gtest/gtest.h>

vt::Vector<int> array = {5, 7, -3, 0, 1};

TEST(VtIntVector, BasicElementAccess) {
  EXPECT_EQ(array[0], 5);
  EXPECT_EQ(array[1], 7);
  EXPECT_EQ(array[4], 1);

  EXPECT_EQ(array.At(0), 5);
  EXPECT_EQ(array.At(2), -3);
  EXPECT_EQ(array.At(4), 1);

  EXPECT_EQ(array.Front(), 5);
  EXPECT_EQ(array.Back(), 1);
}

TEST(VtIntVector, BasicCapacity) {
  EXPECT_FALSE(array.Empty());

  EXPECT_EQ(array.Size(), 5);
}

TEST(VtIntVector, BasicModifiers) {
  array.PopBack();

  EXPECT_EQ(array.Size(), 4);

  int item = -17;
  array.PushBack(item);

  EXPECT_EQ(array.Back(), item);
  EXPECT_EQ(array.Size(), 5);

  item = 3;
  array.PushBack(item);
  array.PushBack(item);
  array.PushBack(item);
  array.PushBack(item);

  EXPECT_EQ(array.Back(), item);
  EXPECT_EQ(array.Size(), 9);
}