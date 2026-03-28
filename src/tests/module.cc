#include <gtest/gtest.h>

#include "../vt/vt.hpp"

TEST(VtIntVector, BasicElementAccess) {
  vt::Vector<int> array = {5, 7, -3, 0, 1};

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
  vt::Vector<int> array = {5, 7, -3, 0, 1};

  EXPECT_FALSE(array.Empty());

  EXPECT_EQ(array.Size(), 5);
}

TEST(VtIntVector, BasicModifiers) {
  vt::Vector<int> array = {5, 7, -3, 0, 1};

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
  vt::Vector<int> array = {5, 7, -3, 0, 1};

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
  vt::Vector<int> array = {5, 7, -3, 0, 1};

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

TEST(VtIntVector, InsertOperation) {
  vt::Vector<int> array = {5, 7, -3, 0, 1};

  auto it1 = array.Insert(array.Begin() + 1, 10);
  EXPECT_EQ(*it1, 10);
  EXPECT_EQ(array[1], 10);

  int val = 20;
  auto it2 = array.Insert(array.Begin(), std::move(val));
  EXPECT_EQ(*it2, 20);
  EXPECT_EQ(array.Front(), 20);

  auto it3 = array.Insert(array.End(), 2, 30);
  EXPECT_EQ(*it3, 30);
  EXPECT_EQ(array.Back(), 30);
  EXPECT_EQ(*(array.End() - 2), 30);

  std::vector<int> range = {40, 50};
  auto it4 = array.Insert(array.Begin() + 2, range.begin(), range.end());
  EXPECT_EQ(*it4, 40);
  EXPECT_EQ(array[2], 40);
  EXPECT_EQ(array[3], 50);

  auto it5 = array.Insert(array.Begin(), {60, 70});
  EXPECT_EQ(*it5, 60);
  EXPECT_EQ(array[0], 60);
  EXPECT_EQ(array[1], 70);
}

TEST(VtIntVector, EraseOperation) {
  vt::Vector<int> c{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  c.Erase(c.Begin());
  EXPECT_EQ(c, (vt::Vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}));

  c.Erase(c.Begin() + 2, c.Begin() + 5);
  EXPECT_EQ(c, (vt::Vector<int>{1, 2, 6, 7, 8, 9}));

  c.Erase(c.End() - 1);
  EXPECT_EQ(c, (vt::Vector<int>{1, 2, 6, 7, 8}));

  for (auto it = c.Begin(); it != c.End();) {
    if (*it % 2 == 0) {
      it = c.Erase(it);
    } else {
      ++it;
    }
  }
  EXPECT_EQ(c, (vt::Vector<int>{1, 7}));
}