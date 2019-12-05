//
// Created by mico on 2019/12/5.
//

#include "test_base.hpp"
#include "common/auto_register.hpp"
#include <unordered_map>
#include <memory>
class MyBase;
std::unordered_map<uint32_t , std::shared_ptr<MyBase>> cmd_map;

struct MyBase {
 public:
 virtual void Do() {}
 static bool Register(uint32_t cmd, std::shared_ptr<MyBase> p) {
   cmd_map.insert(std::make_pair(cmd, p));
   return true;
 }
};

struct A : public AutoRegister<A, 1, MyBase> {
 public:
  A() {}; // 不能省
  void Do() final {
    INFO << "Do A";
  }
};

struct B : public AutoRegister<B, 2, MyBase> {
 public:
  B() {}; // 不能省
  void Do() final {
    INFO << "Do B";
  }
};

TEST_F(auto_register) {
  ASSERT_EQ(cmd_map.size(), 2);
  cmd_map.at(1)->Do();
  cmd_map.at(2)->Do();
}


TEST_FINSH