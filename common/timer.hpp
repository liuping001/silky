//
// Created by liuping on 2020/3/9.
//

// 基于set实现的定时器
#pragma once

#include <set>
#include <functional>
#include <vector>
#include <memory>

class Timer {
 public:
  using TimeOutTask = std::function<void()>;
  struct TimeInfo {
    TimeInfo(TimeOutTask t) : task(std::move(t)){}
    TimeOutTask task;
  };
  using TimerId = std::pair<int64_t, std::shared_ptr<TimeInfo>>;

  ~Timer() {
    Clear();
  }
  TimerId AddTimer(int64_t ms, TimeOutTask &&task) {
    auto ret = timer_set_.emplace(ms, std::make_shared<TimeInfo>(std::move(task)));
    return *ret.first;
  }
  void CancelTimer(const TimerId &timer_id) {
    timer_set_.erase(timer_id);
  }
  void DoTimeOutTask(int64_t now_ms) {
    auto end = timer_set_.lower_bound(TimerId(now_ms + 1, nullptr));
    if (end == timer_set_.begin()) {
      return;
    }
    std::vector<TimerId > time_out_set(timer_set_.begin(), end);
    timer_set_.erase(timer_set_.begin(), end);
    for (auto &item : time_out_set) {
      item.second->task();
    }
  }
  size_t TimerSize() {
    return timer_set_.size();
  }
  void Clear() {
    timer_set_.clear();
  }
 private:
  std::set<TimerId> timer_set_;
};

