//
// Created by liuping on 2020/3/9.
//

// 基于set实现的定时器
#pragma once

#include <set>
#include <functional>
#include <vector>
#include <memory>
#include <unordered_set>

class Timer {
 public:
  using TimeOutTask = std::function<bool ()>; // 返回值表示是否继续加入定时器
  struct TimeInfo {
    TimeInfo(TimeOutTask t) : task(std::move(t)){}
    TimeOutTask task;
    uint32_t interval = 0;
    uint32_t seq = 0;
  };
  using TimerId = std::pair<TimeInfo*, uint32_t>;
  using TimerNode = std::pair<time_t, std::shared_ptr<TimeInfo>>;
  ~Timer() {
    Clear();
  }
  TimerId AddTimer(time_t now, time_t interval, TimeOutTask &&task) {
    auto t = std::make_shared<TimeInfo>(std::move(task));
    t->interval = interval;
    t->seq = now / 1000;
    auto ret = timer_set_.emplace(now + interval, t);
    return TimerId(t.get(), t->seq);
  }
  void CancelTimer(const TimerId &timer_id) {
    cancel_list_.insert(timer_id);
  }
  void DoTimeOutTask(time_t now_ms) {
    auto end = timer_set_.lower_bound(TimerNode(now_ms + 1, nullptr));
    if (end == timer_set_.begin()) {
      return;
    }
    std::vector<TimerNode> time_out_set(timer_set_.begin(), end);
    timer_set_.erase(timer_set_.begin(), end);
    for (auto &item : time_out_set) {
      if (!cancel_list_.empty()) {
        auto iter = cancel_list_.find(TimerId(item.second.get(), item.second->seq));
        if (iter != cancel_list_.end()) {
          cancel_list_.erase(iter);
          continue;
        }
      }
      auto b = item.second->task();
      if (b) {
        timer_set_.emplace(now_ms + item.second->interval, item.second);
      }
    }
  }
  size_t TimerSize() {
    return timer_set_.size();
  }
  void Clear() {
    timer_set_.clear();
  }
 private:
  std::set<TimerNode> timer_set_;
  std::set<TimerId> cancel_list_;
};

