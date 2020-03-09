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
  inline std::vector<std::string> split(const std::string &text, const std::string &delim) {
    return std::vector<std::string> {
        std::sregex_token_iterator(text.begin(), text.end(), std::regex{delim}, -1),
        std::sregex_token_iterator()
    };
  }
}
