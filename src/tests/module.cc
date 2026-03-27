#include <gtest/gtest.h>

#include "../vt/vt.hpp"

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

TEST(VtIntVector, BasicIterators) {
  auto it = array.Begin();
  EXPECT_EQ(*it, 5);
  EXPECT_EQ(*(it + 2), -3);

  int count = 0;
  for (auto i = array.Begin(); i != array.End(); i++) {
    count++;
  }
  EXPECT_EQ(count, array.Size());

  auto last = array.End() - 1;
  EXPECT_EQ(*last, array.Back());

  EXPECT_TRUE(array.Begin() < array.End());
  EXPECT_EQ(array.End() - array.Begin(), array.Size());
}

TEST(VtIntVector, ReverseIterators) {
  auto rit = array.RBegin();
  EXPECT_EQ(*rit, array.Back());
  EXPECT_EQ(*(rit + 1), 0);

  int count = 0;
  for (auto it = array.RBegin(); it != array.REnd(); it++) {
    count++;
  }
  EXPECT_EQ(count, array.Size());

  EXPECT_EQ(*(array.REnd() - 1), array.Front());
  EXPECT_TRUE(array.RBegin() < array.REnd());
}

TEST(VtIntVector, ConstIterators) {
  const vt::Vector<int>& const_array = array;

  auto cit = const_array.CBegin();
  EXPECT_EQ(*cit, 5);
  EXPECT_EQ(*(cit + 2), -3);

  int count = 0;
  for (auto i = const_array.CBegin(); i != const_array.CEnd(); i++) {
    count++;
  }
  EXPECT_EQ(count, const_array.Size());

  auto crit = const_array.CRBegin();
  EXPECT_EQ(*crit, const_array.Back());
  EXPECT_EQ(*(crit + (const_array.Size() - 1)), const_array.Front());
}