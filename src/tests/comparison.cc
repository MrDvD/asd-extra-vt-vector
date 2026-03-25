#include <gtest/gtest.h>

#include <cmath>
#include <exception>
#include <random>

#include "../vt/vt.hpp"

TEST(VtComparison, MassOperations) {
  std::vector<int> std_array = {};
  vt::Vector<int> vt_array = {};

  std::random_device r1, r2;
  std::default_random_engine e1(r1()), e2(r2());
  std::uniform_int_distribution<std::size_t> uniform_cmd(1, 4);
  std::uniform_real_distribution<float> uniform_arg(0.0, 1.0);

  for (std::size_t i = 0; i < 1e6; i++) {
    std::size_t cmd = uniform_cmd(e1);
    switch (cmd) {
      case 1: {
        float rawItem = uniform_arg(e2);
        int item = (int)(rawItem * 1e6);

        std_array.push_back(item);
        vt_array.PushBack(item);
        EXPECT_EQ(std_array.size(), vt_array.Size());
        break;
      }
      case 2: {
        EXPECT_EQ(std_array.size(), vt_array.Size());
        if (vt_array.Size() > 0) {
          int std_front = std_array.front();
          int vt_front = vt_array.Front();
          EXPECT_EQ(std_front, vt_front);

          int vt_back = vt_array.Back();
          int std_back = std_array.back();
          EXPECT_EQ(std_back, vt_back);
        }
        break;
      }
      case 3:
        EXPECT_EQ(std_array.size(), vt_array.Size());
        if (vt_array.Size() > 0) {
          std_array.pop_back();
          vt_array.PopBack();
          EXPECT_EQ(std_array.size(), vt_array.Size());
        }
        break;
      case 4: {
        EXPECT_EQ(std_array.size(), vt_array.Size());
        if (vt_array.Size() > 0) {
          float rawIdx = uniform_arg(e2);
          int idx = (int)(rawIdx * (vt_array.Size() - 1));
          int std_item = std_array.at(idx);
          int vt_item = vt_array.At(idx);
          EXPECT_EQ(std_item, vt_item);
        }
        break;
      }
      default:
        throw std::runtime_error("undefined command");
    }
  }
}