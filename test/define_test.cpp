//
// Created by liuping on 2019/3/27.
//

#include "common/define.hpp"
#include "test_base.hpp"

TEST_F(scope) {
    int i = 0;
    SCOPE {
        i++;
        ASSERT_EQ(i, 1);
        if (i == 1) {
            break;
        }
        // 运行不到这里来
        ASSERT_EQ(i, 0);
    };

    i = 0;

    IF(i == 0) {
        INFO << "if i=" << i;
        break;
        ASSERT(false);
    }
    ELSE_IF(i == 1) {
        INFO << "else if i=" << i;
        break;
        ASSERT(false);
    }
    ELSE {
        INFO << "else i=" << i;
        break;
        ASSERT(false);
    };

    i = 1;
    IF(i == 0) {
        INFO << "if i=" << i;
        break;
        ASSERT(false);
    }
    ELSE_IF(i == 1) {
        INFO << "else if i=" << i;
        break;
        ASSERT(false);
    }
    ELSE {
        INFO << "else i=" << i;
        break;
        ASSERT(false);
    };

    i = 2;
    IF(i == 0) {
        INFO << "if i=" << i;
        break;
        ASSERT(false);
    }
    ELSE_IF(i == 1) {
        INFO << "else if i=" << i;
        break;
        ASSERT(false);
    }
    ELSE {
        INFO << "else i=" << i;
        break;
        ASSERT(false);
    };
}

TEST_FINSH