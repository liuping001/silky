//
// Created by liuping on 2020/3/9.
//

#include "test_base.hpp"

#include "common/timer.hpp"

TEST_F(my_timer) {
  Timer timer;
  timer.AddTimer(1000, 1, [](){
    cout << "hello world\n";
    return false;
  });
  timer.DoTimeOutTask(1001);
  timer.AddTimer(1002, 1, []() {
    cout << "!!!\n";
    return false;
  });
  auto timer_id = timer.AddTimer(1002, 1, []() {
    cout << "!!!\n";
    return false;
  });
  timer.CancelTimer(timer_id);
  timer.DoTimeOutTask(1003);

  timer.AddTimer(1003, 1, [](){
    cout << "timer 1\n";
    return true;
  });
  timer_id = timer.AddTimer(1003, 1, []() {
    cout << "timer 2\n";
    return true;
  });
  timer.DoTimeOutTask(1004);
  timer.DoTimeOutTask(1005);
  timer.DoTimeOutTask(1006);
  timer.CancelTimer(timer_id);
  timer.DoTimeOutTask(1007);
  timer.DoTimeOutTask(1008);
}

TEST_FINSH