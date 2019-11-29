//
// Created by liuping on 2019/4/2.
//
#include <string>
#include "test_base.h"
#include "common/scope_guard.h"

int func(int i) {
    WARN << "normal func " << i;
    return 0;
}

struct  C {
    int operator ()() {
        WARN << "call object ";
        return 0;
    }
};

TEST_F(scope_guard) {
    {
        int i = 0;
        auto defer = make_guard([&i] {
            i++;
            WARN << "i:" << i;
            ASSERT_EQ(i, 3);
        });
        i = 2;
    }

    make_guard(nullptr);

    make_guard([]() -> int { return 0;}); // 带返回值的也可以

    {
        auto f = []() {
            WARN << "lambda";
        };
        auto defer = make_guard(f);
        INFO << "test lambda";
    }

    {
        auto defer = make_guard(std::bind(func, 100));
        INFO << "test normal func";
    }

    {
        auto defer = make_guard(C());
        INFO << "test call object";
    }
}

TEST_FINSH