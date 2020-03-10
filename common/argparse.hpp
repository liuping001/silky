//
// Created by liuping on 2020/3/10.
//

#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <regex>
#include <iterator>
#include <iostream>
#include <cstdio>
#include <set>

inline std::vector<std::string> split(const std::string &text, const std::string &delim) {
  return std::vector<std::string> {
      std::sregex_token_iterator(text.begin(), text.end(), std::regex{delim}, -1),
      std::sregex_token_iterator()
  };
}
struct Arg {
  std::string short_key;
  std::string key;
  std::string help;
  bool require;
  std::string value;
};

class ParseException : std::exception {
  std::string msg_;
 public:
  ParseException(std::string msg) : msg_(std::move(msg)) {};
  const char* what() const noexcept { return msg_.c_str(); }
};

class ParseDebugException : std::exception {
  std::string msg_;
 public:
  ParseDebugException(std::string msg) : msg_(std::move(msg)) {};
  const char* what() const noexcept { return msg_.c_str(); }
};


class ArgumentParser {
  int argc_;
  char **argv_;
  std::unordered_map<std::string, std::shared_ptr<Arg>> arg_map_;
 public:
  ArgumentParser(int argc, char ** argv) : argc_(argc), argv_(argv) {}
  void AddArgument(Arg arg) {
    auto p = std::make_shared<Arg>(arg);
    arg_map_.emplace(arg.short_key.substr(1), p);
    arg_map_.emplace(arg.key.substr(2), p);
  }

  std::string Get(const std::string &key) const {
    auto iter = arg_map_.find(key);
    if (iter == arg_map_.end()) {
      return {};
    }
    return iter->second->value;
  }

  void Debug() const {
    printf("usage:\n");
    printf("* is required arg. support format: -c xxx -c=xxx --conf xxx --conf = xxx\n");
    auto all_args = AllArg();
    for (auto &item : all_args) {
      printf("    %c %-5s%-15s : %s\n", item->require ? '*':' ', item->short_key.c_str(), item->key.c_str(), item->help.c_str());
    }
  }

  void ParseArgs() {
    if (argc_ < 2) {
      return;
    }
    std::string all_args;
    for (int i = 1; i < argc_; i++) {
      all_args.append(" ");
      all_args.append(argv_[i]);
    }

    std::unordered_map<std::string,std::string> arg_kv;
    try {
      auto echo_args = split(all_args, "\\s+-+");
      for (auto &item : echo_args) {
        if (item.empty()) {
          continue;
        }
        auto key_value = split(item, "\\s+|\\s*=\\s*");
        if (key_value.size() != 2) {
          if (!key_value.empty() && (key_value[0] == "h" ||key_value[0] == "help")) {
            throw ParseDebugException("");
          } else {
            throw ParseException("parse failed: " + item);
          }
        }
        arg_kv[key_value[0]] = key_value[1];
      }

      CheckRequire(arg_kv);
      SetArg(arg_kv);
    } catch (ParseDebugException e) {
      std::cout << e.what() <<"\n";
      Debug();
      exit(0);
    } catch (ParseException e) {
      std::cout << e.what() <<"\n";
      exit(0);
    }
  }

 private:
  void CheckRequire(const std::unordered_map<std::string, std::string> &arg_kv) {
    std::string require_list;
    auto all_args = AllArg();
    for (auto &item : all_args) {
      if (!item->require) {
        continue;
      }
      auto iter_1 = arg_kv.find(item->short_key.substr(1));
      auto iter_2 = arg_kv.find(item->key.substr(2));
      if (iter_1 == arg_kv.end() && iter_2 == arg_kv.end()) {
        require_list.append(item->key);
        require_list.append(" ");
      }
    }
    if (!require_list.empty()) {
      throw ParseException("required arg: " + require_list);
    }
  }

  void SetArg(const std::unordered_map<std::string, std::string> &arg_kv) {
    for (auto &item : arg_kv) {
      auto iter = arg_map_.find(item.first);
      if (iter == arg_map_.end()) {
        throw ParseDebugException("no define arg:" + item.first);
      }
      iter->second->value = item.second;
    }
  }

  std::set<std::shared_ptr<Arg>> AllArg() const {
    std::set<std::shared_ptr<Arg>> all_args;
    for (const auto &item : arg_map_) {
      all_args.insert(item.second);
    }
    return all_args;
  }
};
