//
// Created by liuping on 2020/3/7.
//

#include "test_base.hpp"
#include "common/shrong_string.hpp"

TEST_F(split_ip) {
  auto ret = sstr::split("192.168.0.1", "\\.");
  for (auto item : ret) {
    INFO << item << "\n";
  }
  ASSERT_EQ(ret.size(), 4);
  ASSERT_EQ(ret.at(0), "192");
  ASSERT_EQ(ret.at(1), "168");
  ASSERT_EQ(ret.at(2), "0");
  ASSERT_EQ(ret.at(3), "1");
}

TEST_FINSH