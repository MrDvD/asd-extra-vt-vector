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

TEST(VtIntVector, ResizeAndReserve) {
  vt::Vector<int> array = {1, 2, 3};

  array.Reserve(10);
  EXPECT_GE(array.Capacity(), 10);
  EXPECT_EQ(array.Size(), 3);
  EXPECT_EQ(array[0], 1);
  EXPECT_EQ(array[1], 2);
  EXPECT_EQ(array[2], 3);

  array.Resize(5);
  EXPECT_EQ(array.Size(), 5);
  EXPECT_EQ(array[3], 0);
  EXPECT_EQ(array[4], 0);

  array.Resize(2);
  EXPECT_EQ(array.Size(), 2);
  EXPECT_EQ(array[0], 1);
  EXPECT_EQ(array[1], 2);

  array.Resize(4, 99);
  EXPECT_EQ(array.Size(), 4);
  EXPECT_EQ(array[0], 1);
  EXPECT_EQ(array[1], 2);
  EXPECT_EQ(array[2], 99);
  EXPECT_EQ(array[3], 99);

  std::size_t current_cap = array.Capacity();
  array.Reserve(current_cap / 2);
  EXPECT_EQ(array.Capacity(), current_cap);

  array.Resize(0);
  EXPECT_TRUE(array.Empty());
  EXPECT_EQ(array.Size(), 0);
}

TEST(VtIntVector, Constructors) {
  vt::Vector<int> v1;
  EXPECT_TRUE(v1.Empty());
  EXPECT_EQ(v1.Size(), 0);

  vt::Vector<int> v3(5);
  EXPECT_EQ(v3.Size(), 5);
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(v3[i], 0);
  }

  vt::Vector<int> v4(3, 21);
  EXPECT_EQ(v4.Size(), 3);
  EXPECT_EQ(v4[0], 21);
  EXPECT_EQ(v4[1], 21);
  EXPECT_EQ(v4[2], 21);

  std::vector<int> source = {10, 20, 30};
  vt::Vector<int> v5(source.begin(), source.end());
  EXPECT_EQ(v5.Size(), 3);
  EXPECT_EQ(v5[0], 10);
  EXPECT_EQ(v5[2], 30);

  vt::Vector<int> v7(v4);
  EXPECT_EQ(v7.Size(), v4.Size());
  EXPECT_EQ(v7[0], 21);

  vt::Vector<int> move_source = {1, 2, 3};
  vt::Vector<int> v8(std::move(move_source));
  EXPECT_EQ(v8.Size(), 3);
  EXPECT_EQ(v8[0], 1);

  vt::Vector<int> v9(v5, std::allocator<int>());
  EXPECT_EQ(v9.Size(), 3);
  EXPECT_EQ(v9[1], 20);

  vt::Vector<int> move_alloc_source = {7, 8, 9};
  vt::Vector<int> v10(std::move(move_alloc_source), std::allocator<int>());
  EXPECT_EQ(v10.Size(), 3);
  EXPECT_EQ(v10[2], 9);

  vt::Vector<int> v11 = {100, 200, 300, 400};
  EXPECT_EQ(v11.Size(), 4);
  EXPECT_EQ(v11[0], 100);
  EXPECT_EQ(v11[3], 400);
}