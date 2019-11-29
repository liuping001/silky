//
// Created by liuping on 2019/3/27.
//

#include "test_base.h"
#include "common/object_pool.h"
class A {

};

TEST_F(test_object_pool) {
    object_pool<A> p;
    p.add(std::unique_ptr<A>(new A()));
    p.add(std::unique_ptr<A>(new A()));
    {
        auto t = p.get();
        ASSERT_EQ(p.size(), 1);
        p.get();
        ASSERT_EQ(p.size(), 1);
    }
    ASSERT_EQ(p.size(), 2);
    {
        p.get();
        ASSERT_EQ(p.size(), 2);
        p.get();
        ASSERT_EQ(p.size(), 2);
    }
    ASSERT_EQ(p.size(), 2);
    std::cout << p.size() << std::endl;
}

TEST_FINSH