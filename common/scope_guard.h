//
// Created by liuping on 2019/4/2.
//

#pragma once
#include <type_traits>

template <typename F>
class scope_guard
{
public:
    explicit scope_guard(F && f) : func_(std::move(f)), cancel_(false) {}
    explicit scope_guard(const F& f) : func_(f), cancel_(false) {}

    ~scope_guard() {
        if (!cancel_) {
            func_();
        }
    }

    scope_guard(scope_guard && rhs) : func_(std::move(rhs.func_)), cancel_(rhs.cancel_) {
        rhs.release();
    }

    void release() {
        cancel_ = true;
    }

 private:
    F func_;
    bool cancel_;

    scope_guard(const scope_guard&) = delete;
    scope_guard&operator=(const scope_guard&) = delete;
};

template <>
struct scope_guard <std::nullptr_t> {
    explicit scope_guard(std::nullptr_t &&) {}
    explicit scope_guard(const std::nullptr_t&) {}
};

template <typename F> scope_guard<typename std::decay<F>::type> make_guard(F && f)
{
    return scope_guard<typename std::decay<F>::type>(std::forward<F>(f));
}
