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
  std::uniform_int_distribution<std::size_t> uniform_cmd(1, 8);
  std::uniform_real_distribution<float> uniform_arg(0.0, 1.0);

  for (std::size_t i = 0; i < 20000; i++) {
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
        if (vt_array.Size() == 0) {
          continue;
        }
        int std_front = std_array.front();
        int vt_front = vt_array.Front();
        EXPECT_EQ(std_front, vt_front);

        int vt_back = vt_array.Back();
        int std_back = std_array.back();
        EXPECT_EQ(std_back, vt_back);
        break;
      }
      case 3:
        EXPECT_EQ(std_array.size(), vt_array.Size());
        if (vt_array.Size() == 0) {
          continue;
        }
        std_array.pop_back();
        vt_array.PopBack();
        EXPECT_EQ(std_array.size(), vt_array.Size());
        break;
      case 4: {
        EXPECT_EQ(std_array.size(), vt_array.Size());
        if (vt_array.Size() == 0) {
          continue;
        }
        float rawIdx = uniform_arg(e2);
        int idx = (int)std::round(rawIdx * (vt_array.Size() - 1));
        int std_item = std_array.at(idx);
        int vt_item = vt_array.At(idx);
        EXPECT_EQ(std_item, vt_item);
        break;
      }
      case 5: {
        EXPECT_EQ(std_array.size(), vt_array.Size());
        if (vt_array.Size() == 0) {
          continue;
        }
        auto std_it = std_array.begin();
        auto vt_it = vt_array.Begin();
        auto std_rit = std_array.rbegin();
        auto vt_rit = vt_array.RBegin();
        EXPECT_EQ(*std_it, *vt_it);
        EXPECT_EQ(*std_rit, *vt_rit);

        float rawIdx = uniform_arg(e2);
        int idx = (int)std::round(rawIdx * (vt_array.Size() - 1));
        EXPECT_EQ(*(std_it + idx), *(vt_it + idx));
        EXPECT_EQ(*(std_rit + idx), *(vt_rit + idx));
        EXPECT_EQ(std_array.end() - (std_it + idx), vt_array.End() - (vt_it + idx));
        break;
      }
      case 6: {
        int offset = vt_array.Size() == 0 ? 0 : (int)std::round(uniform_arg(e2) * vt_array.Size());
        int insert_subtype = (int)std::round(uniform_arg(e2) * 3.0);
        int item = (int)(uniform_arg(e2) * 1e6);
        std::size_t count = (std::size_t)(uniform_arg(e2) * 5) + 1;
        switch (insert_subtype) {
          case 0: {
            auto std_it = std_array.insert(std_array.begin() + offset, item);
            auto vt_it = vt_array.Insert(vt_array.Begin() + offset, item);
            EXPECT_EQ(*std_it, *vt_it);
            break;
          }
          case 1: {
            auto std_it = std_array.insert(std_array.begin() + offset, count, item);
            auto vt_it = vt_array.Insert(vt_array.Begin() + offset, count, item);
            EXPECT_EQ(*std_it, *vt_it);
            break;
          }
          case 2: {
            std::vector<int> source_vec;
            for (std::size_t i = 0; i < count; i++) {
              source_vec.push_back((int)(uniform_arg(e2) * 1e6));
            }
            auto std_it =
                std_array.insert(std_array.begin() + offset, source_vec.begin(), source_vec.end());
            auto vt_it =
                vt_array.Insert(vt_array.Begin() + offset, source_vec.begin(), source_vec.end());
            EXPECT_EQ(*std_it, *vt_it);
            break;
          }
          case 3: {
            int val1 = (int)(uniform_arg(e2) * 1e6);
            int val2 = (int)(uniform_arg(e2) * 1e6);
            auto ilist = {val1, val2};
            auto std_it = std_array.insert(std_array.begin() + offset, ilist);
            auto vt_it = vt_array.Insert(vt_array.Begin() + offset, ilist);
            EXPECT_EQ(*std_it, *vt_it);
            break;
          }
        }
        break;
      }
      case 7: {
        EXPECT_EQ(std_array.size(), vt_array.Size());
        if (vt_array.Size() == 0) {
          continue;
        }

        int erase_subtype = (int)std::round(uniform_arg(e2) * 1.0);
        if (erase_subtype == 0) {
          int idx = (int)std::round(uniform_arg(e2) * (vt_array.Size() - 1));
          auto std_it = std_array.erase(std_array.begin() + idx);
          auto vt_it = vt_array.Erase(vt_array.Begin() + idx);

          if (std_it != std_array.end()) {
            EXPECT_EQ(*std_it, *vt_it);
          }
          EXPECT_EQ(std_array.end() - std_it, vt_array.End() - vt_it);
        } else {
          int idx1 = (int)std::round(uniform_arg(e2) * vt_array.Size());
          int idx2 = idx1 + (int)std::round(uniform_arg(e2) * (vt_array.Size() - idx1));

          auto std_it = std_array.erase(std_array.begin() + idx1, std_array.begin() + idx2);
          auto vt_it = vt_array.Erase(vt_array.Begin() + idx1, vt_array.Begin() + idx2);

          if (std_it != std_array.end()) {
            EXPECT_EQ(*std_it, *vt_it);
          }
          EXPECT_EQ(std_array.end() - std_it, vt_array.End() - vt_it);
        }
        break;
      }
      case 8: {
        bool use_value = uniform_arg(e2) > 0.5f;
        std::size_t current_size = vt_array.Size();
        std::size_t new_size;
        float size_factor = uniform_arg(e2) * 2.0f;
        new_size = (std::size_t)(current_size * size_factor);
        if (use_value) {
          int fill_value = (int)(uniform_arg(e2) * 1e6);
          std_array.resize(new_size, fill_value);
          vt_array.Resize(new_size, fill_value);
        } else {
          std_array.resize(new_size);
          vt_array.Resize(new_size);
        }
        EXPECT_EQ(std_array.size(), vt_array.Size());
        if (new_size > 0) {
          EXPECT_EQ(std_array.back(), vt_array.Back());
          EXPECT_EQ(std_array.front(), vt_array.Front());
        }
        break;
      }
      default:
        throw std::runtime_error("undefined command");
    }
  }
}