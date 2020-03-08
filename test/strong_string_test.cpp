//
// Created by liuping on 2020/3/7.
//

#include "test_base.hpp"
#include "common/shrong_string.hpp"

TEST_F(split_ip) {
  auto ret = sstr::split("192.168.0.1", "\\.");
  for (auto &item : ret) {
    INFO << item;
  }
  ASSERT_EQ(ret.size(), 4);
  ASSERT_EQ(ret.at(0), "192");
  ASSERT_EQ(ret.at(1), "168");
  ASSERT_EQ(ret.at(2), "0");
  ASSERT_EQ(ret.at(3), "1");
}

TEST_F(split_str) {
  auto ret = sstr::split("A10;S20;W10;D30;X;A1A;B10A11;;A10;", ";+");
  for (auto &item : ret) {
    INFO << item;
  }
  ASSERT_EQ(ret.size(), 8);
}

TEST_FINSH