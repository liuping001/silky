#pragma once

#include <string>

namespace type {

template<class T> inline T string_to(const std::string &str);

template<> inline long string_to<long>(const std::string &str) { return std::stol(str); }
template<> inline int string_to<int>(const std::string &str) { return std::stoi(str); }
template<> inline double string_to<double>(const std::string &str) { return std::stod(str); }
template<> inline float string_to<float>(const std::string &str) { return std::stof(str); }
template<> inline uint32_t string_to<uint32_t>(const std::string &str) { return std::stoul(str); }

inline std::string to_string(const std::string &str) { return str;}

template<class T> inline std::string to_string(T t) { return std::to_string(t); }

}