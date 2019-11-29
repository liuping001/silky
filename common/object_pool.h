//
// Created by liuping on 2019/3/27.
//
// 出自:https://github.com/qicosmos/cosmos/blob/master/object_pool/SimpleObjectPool.hpp
#pragma once

#include <memory>
#include <vector>
#include <functional>

template <class T>
class object_pool
{
public:
    using DeleterType = std::function<void(T*)>;

    void add(std::unique_ptr<T> t)
    {
        pool_.push_back(std::move(t));
    }

    std::unique_ptr<T, DeleterType> get()
    {
        if (pool_.empty())
        {
            throw std::logic_error("no more object");
        }

        //every time add custom deleter for default unique_ptr
        std::unique_ptr<T, DeleterType> ptr(pool_.back().release(), [this](T* t)
        {
            pool_.push_back(std::unique_ptr<T>(t));
        });

        pool_.pop_back();
        return std::move(ptr);
    }

    std::shared_ptr<T> get_shared()
    {
        if (pool_.empty())
        {
            throw std::logic_error("no more object");
        }

        auto pin = std::unique_ptr<T>(std::move(pool_.back()));
        pool_.pop_back();

        return std::shared_ptr<T>(pin.release(), [this](T* t)
        {
            pool_.push_back(std::unique_ptr<T>(t));
        });
    }

    bool empty() const
    {
        return pool_.empty();
    }

    size_t size() const
    {
        return pool_.size();
    }

private:
    std::vector<std::unique_ptr<T>> pool_;
};


