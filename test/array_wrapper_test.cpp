//
// Created by liuping on 2019/3/11.
//
#include "test_base.h"
#include "common/array_wrapper.h"
#include <algorithm>

using namespace array;
struct TestInfo {
    TestInfo(){}
    TestInfo(int _id, int _level) : id(_id), level(_level) {}
    int id = 0;
    int level = 0;
};

constexpr size_t kMaxLength = 10;

struct TestArrayWrapper {
    // data
    TestInfo info_[kMaxLength];
    size_t info_length_ = 0;

    template <typename T> void CheckEmpty(T wrapper) {
        ASSERT_EQ(wrapper.length(), 0);
        ASSERT_EQ(wrapper.capacity(), kMaxLength);
        ASSERT_EQ(wrapper.at(0), nullptr);
        ASSERT_EQ(wrapper.check_add(), true);
        ASSERT_EQ(wrapper.begin(), info_);
        ASSERT_EQ(wrapper.end(), info_);
    }

    template <typename T> void CheckFull(T wrapper) {
        ASSERT_EQ(wrapper.length(), kMaxLength);
        ASSERT_EQ(wrapper.capacity(), kMaxLength);
        ASSERT_EQ(wrapper.at(kMaxLength), nullptr);
        ASSERT_NE(wrapper.at(kMaxLength - 1), nullptr);
        ASSERT_EQ(wrapper.check_add(), false);
        ASSERT_EQ(wrapper.begin(), info_);
        ASSERT_EQ(wrapper.end(), info_ + kMaxLength);
    }
};

TEST(TestArrayWrapper, Empty) {
    auto wrapper = make_wrapper(info_, info_length_);
    CheckEmpty(wrapper);
}

TEST(TestArrayWrapper, Add) {
    auto wrapper = make_wrapper(info_, info_length_);
    for (size_t i = 0; i < kMaxLength; i++) {
        auto iter = wrapper.add();
        ASSERT(iter != nullptr);
        iter->id = 1 + i;
        iter->level = 11 + i;
        ASSERT_EQ(wrapper.length(), i + 1);
        ASSERT_EQ(wrapper.length(), info_length_);
    }

    ASSERT_EQ(wrapper.add(), nullptr);
    for (size_t i = 0; i < kMaxLength; i++) {
        auto iter = wrapper.at(i);
        ASSERT(iter != nullptr);
        ASSERT_EQ(iter->id, 1 + i);
        ASSERT_EQ(iter->level, 11 + i);
    }
    ASSERT_NE(wrapper.at(kMaxLength - 1), nullptr);
    ASSERT_EQ(wrapper.at(kMaxLength), nullptr);

    auto wrapper2 = make_wrapper(info_, info_length_);
    wrapper2.clear();

    TestInfo info1;
    info1.id = 1001;
    info1.level = 1002;
    wrapper2.add(info1);
    ASSERT_EQ(wrapper2.length(), 1);
    ASSERT_EQ(wrapper2.at(0)->id, info1.id);
    ASSERT_EQ(wrapper2.at(0)->level, info1.level);
}

TEST(TestArrayWrapper, Erase) {
    auto wrapper = make_wrapper(info_, info_length_);
    for (size_t i = 0; i < kMaxLength; i++) {
        auto iter = wrapper.add();
        iter->id = 1 + i; // [1,2,3,4,5,6,7,8,9,10]
        iter->level = 11 + i;
    }
    CheckFull(wrapper);

    // 删除第一个元素
    wrapper.erase(wrapper.begin());
    ASSERT_EQ(wrapper.length(), kMaxLength - 1);
    {
        int i = 1;
        for (auto iter : wrapper) {
            ASSERT_EQ(iter.id, 1 + i);
            ASSERT_EQ(iter.level, 11 + i);
            i ++;
        }
    }
    // 还剩[2,3,4,5,6,7,8,9,10]

    // 删除最后一个元素
    wrapper.erase(wrapper.end() - 1);
    ASSERT_EQ(wrapper.length(), kMaxLength - 2);
    {
        int i = 1;
        for (auto iter : wrapper) {
            ASSERT_EQ(iter.id, 1 + i);
            ASSERT_EQ(iter.level, 11 + i);
            i ++;
        }
    }
    // 还剩[2,3,4,5,6,7,8,9]
    // 删除一个不存在的元素
    wrapper.erase(wrapper.end());
    ASSERT_EQ(wrapper.length(), kMaxLength - 2);

    // 删除id为偶数的
    auto new_end = std::remove_if(wrapper.begin(), wrapper.end(), [](const decltype(wrapper)::type &item){
        return item.id % 2 == 0;
    });

    wrapper.erase(new_end, wrapper.end());
    // 还剩[3,5,7,9]
    std::vector<int> remain_num = {3, 5, 7, 9};
    ASSERT_EQ(wrapper.length(), remain_num.size());
    ASSERT_EQ(wrapper.at(remain_num.size()), nullptr);
    ASSERT_EQ(wrapper.length(), info_length_);
    for (size_t i = 0; i < wrapper.length(); i++) {
        ASSERT_EQ(wrapper.at(i)->id, remain_num[i]);
        ASSERT_EQ(wrapper.at(i)->level, remain_num[i] + 10);
    }

    // 清空再加满元素
    wrapper.erase(wrapper.begin(), wrapper.end());
    CheckEmpty(wrapper);
    for (size_t i = 0; i < kMaxLength; i++) {
        auto iter = wrapper.add();
        iter->id = 1 + i; // [1,2,3,4,5,6,7,8,9,10]
        iter->level = 11 + i;
    }
    CheckFull(wrapper);
}

TEST(TestArrayWrapper, Find) {
    auto wrapper = make_wrapper(info_, info_length_);
    if (wrapper.check_add()) {
        ASSERT_NE(wrapper.add(1, 1), nullptr);
    }
    if (wrapper.check_add()) {
        ASSERT_NE(wrapper.add(3, 3), nullptr);
    }
    ASSERT_EQ(wrapper.length(), 2);

    auto ret_1 = std::find_if(wrapper.begin(), wrapper.end(),
                              [](const decltype(wrapper)::type &item) { return item.id == 1; });
    ASSERT_EQ(ret_1, wrapper.at(0));
    ASSERT_EQ(ret_1->id, 1);
    ASSERT_EQ(ret_1->level, 1);

    auto ret_2 = std::find_if(wrapper.begin(), wrapper.end(),
                              [](const decltype(wrapper)::type &item) { return item.id == 2; });
    ASSERT_EQ(ret_2, wrapper.end());
}

TEST_FINSH