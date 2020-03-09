//
// Created by liuping on 2020/3/9.
//

#include "test_base.hpp"

#include "common/timer.hpp"

TEST_F(my_timer) {
  Timer timer;
  timer.AddTimer(1000, [](){ cout << "hello world\n";});
  timer.DoTimeOutTask(1001);
  timer.AddTimer(1002, []() { cout << "!!!\n";});
  auto timer_id = timer.AddTimer(1002, []() { cout << "!!!\n";});
  timer.CancelTimer(timer_id);
  timer.DoTimeOutTask(1003);
}

TEST_FINSH