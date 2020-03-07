//
// Created by liuping on 2020/3/7.
//

#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <iterator>
#include <regex>

namespace sstr {
  static std::vector<std::string> split(const std::string &text, const std::string &re) {
    std::vector<std::string> ret;
    std::copy(std::sregex_token_iterator(text.begin(), text.end(), std::regex(re), -1),
              std::sregex_token_iterator(),
              std::back_inserter(ret));
    return ret;
  }
}
