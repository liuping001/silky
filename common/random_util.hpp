
/***********************************
 * 随机函数工具集
 * 特点:
 * 根据策略支持多线程共用一个随机数种子，也支持每线程一个独立的随机种子
 *
 ***********************************/
#pragma once
#include <random>
#include <stdexcept>
#include <vector>
#include <mutex>

namespace random_util {

struct RandGen {
  std::mt19937 gen;
  std::uniform_real_distribution<double> dist;
  RandGen() :gen(std::random_device()()), dist(0, 1) {}
  double operator ()() {
    return dist(gen);
  }
};
// 区间[0, 1)
struct Random {
  static double Rand01() {
    static thread_local RandGen rand_gen;
    return rand_gen();
  }
};

// 区间[0, 1)
struct MutexRandom {
  static double Rand01() {
    static RandGen rand_gen;
    static std::mutex g_mutex;
    g_mutex.lock();
    auto ret = rand_gen();
    g_mutex.unlock();
    return ret;
  }
};

template <class R>
class RandomUtil {
 public:
  // 区间[0, high)
  static inline uint32_t Random(uint32_t high) {
    return static_cast<uint32_t>(high * R::Rand01());
  }

  // 区间[low, high)
  static inline uint32_t Random(uint32_t low, uint32_t high) {
    if (low > high) {
      throw std::runtime_error("low > high");
    }
    return low + static_cast<uint32_t>(Random(high - low));
  }

  // 根据weight随机1个
  static size_t WeightedRandomSelect(const std::vector<int> &weight_list) {
    if (weight_list.empty()) {
      throw std::runtime_error("weight_list empty()");
    }
    uint32_t total_weight = std::accumulate(weight_list.begin(), weight_list.end(), 0);
    auto r = Random(total_weight);

    auto tmp_weight = total_weight;
    for (size_t i = 0; i < weight_list.size(); ++i) {
      tmp_weight -= weight_list[i];
      if (r >= tmp_weight) {
        return i;
      }
    }
    return weight_list.size() - 1;
  }

  // 带权随机出m(>=1)个
  static std::vector<size_t> WeightedRandomSelect(std::vector<int> &weight_list, uint32_t m) {
    if (m <= 0 || weight_list.size() <= 0) {
      throw std::runtime_error("weight_list size not enough");
    }
    std::vector<size_t> idx_list;
    if (size_t(m) >= weight_list.size()) { // 全选
      for (size_t i = 0; i < weight_list.size(); ++i) {
        idx_list.push_back(i);
      }
      return idx_list;
    }

    auto weight_list_temp(weight_list);
    for (size_t i = 0; i < m; ++i) {
      auto idx = WeightedRandomSelect(weight_list_temp);
      idx_list.push_back(idx);
      weight_list_temp[idx] = 0;
    }
    return idx_list;
  }
};

} // end namespace random_util

using local_random = random_util::RandomUtil<random_util::Random>;
using global_random = random_util::RandomUtil<random_util::MutexRandom>;
