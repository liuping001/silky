//
// Created by 刘平 on 2019/3/6.
//

#pragma once

#include <stddef.h>
#include <cstring>
#include <type_traits>
#include <functional>
/**
 *array_wrapper是原始数组工具，操作像下面这样的infos数组与infos_length的组合
 *  struct A {...}
 *  struct Test {
 *      A infos[10];
 *      size_t infos_length = 0;
 *  }
 *特点：
 *  1、与stl中的算法兼容
 *  2、支持for(auto &item : array_wrapper)语法
 *  3、add时调用构造函数（参数转发，原址构造），erase时调用析构函数。
 */
namespace array {

template<typename Iter, typename Length>
struct array_wrapper {
    template<typename T, typename L, size_t S>
    friend array_wrapper<T *, L> make_wrapper(T (&begin)[S], L &length); // 限定 只有make_wrapper函数可以构造array_wrapper

    using iter_type = Iter;
    using type = typename std::remove_pointer<Iter>::type;

    Iter begin() { return begin_; }

    Iter end() { return begin_ + used_size_; }

    bool check_add() { return is_valid(used_size_); }

    size_t capacity() { return capacity_; }

    Length length() { return used_size_; }

    Iter at(size_t pos) {
        if (is_use(pos)) {
            return begin_ + pos;
        }
        return Iter();
    }

    template<typename... Args>
    Iter add(Args &&... args) {
        if (check_add()) {
            auto old_use = used_size_;
            used_size_++;
            new(begin_ + old_use) type(std::forward<Args>(args)...); // placement new
            return begin_ + old_use;
        }
        return Iter();
    }

    void clear() { erase(begin(), end()); }

    void erase(Iter iter) { erase(iter, iter + 1); }

    void erase(Iter iter_begin, Iter iter_end) {
        if (iter_begin >= begin() && iter_begin < end() &&
            iter_end > begin() && iter_end <= end() &&
            iter_begin < iter_end) {

            // 调用析构函数
            for (auto iter = iter_begin; iter != iter_end; iter++) {
                iter->~type();
            }

            for (; iter_end != end(); iter_begin++, iter_end++) {
                *iter_begin = *iter_end;
            }
            used_size_ -= iter_end - iter_begin;
        }
    }

 private:
    explicit array_wrapper(Iter begin, Length &used_size, size_t capacity) :
            begin_(begin), used_size_(used_size), capacity_(capacity) {};

    bool is_valid(size_t pos) { return pos >= 0 && pos < capacity_; }

    bool is_use(size_t pos) { return used_size_ == 0 ? false : pos < used_size_; }

 private:
    Iter begin_;
    Length &used_size_;
    const size_t capacity_;
};

template<typename T, typename L, size_t S>
array_wrapper<T *, L> make_wrapper(T (&begin)[S], L &length) {
    return array_wrapper<T *, L>(begin, length, S);
}

} // end namespace array
