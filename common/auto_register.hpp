#pragma once
#include <cstdint>
#include <unordered_map>
#include <memory>

template <class T, uint32_t cmd, class Base>
struct AutoRegister : public Base {
  AutoRegister() {
    reg;
  }

  static bool reg;
};

template <class T, uint32_t cmd, class Base>
bool AutoRegister<T, cmd, Base>::reg = AutoRegister<T, cmd, Base>::Register(cmd, std::make_shared<T>());