#include <gtest/gtest.h>
#include "../vt/vt.h"

vt::vector<int> h = {5, 7, -3, 0, 1};

TEST(VtVector, Reading) {
  EXPECT_EQ(h[0], 5);
  EXPECT_EQ(h[1], 7);
  EXPECT_EQ(h[4], 1);

  EXPECT_EQ(h.at(0), 5);
  EXPECT_EQ(h.at(2), -3);
  EXPECT_EQ(h.at(4), 1);

  EXPECT_EQ(h.front(), 5);
  EXPECT_EQ(h.back(), 1);
}