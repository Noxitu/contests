#pragma once
#include <memory>
#include <array>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <chrono>

struct Buffer
{
    std::array<char, 256*1024> buffer_internal;
    std::array<char, 256*1024>::iterator buffer = std::begin(buffer_internal);
    std::chrono::duration<double> timestamp;

    template<typename Value>
    void put(Value const &value)
    {
        auto src = reinterpret_cast<char const*>(&value);
        buffer = std::copy(src, src+sizeof(Value), buffer);
    }

    template<typename Value>
    Value get()
    {
        assert(buffer+sizeof(Value) <= end(buffer_internal));
        Value value;
        auto dst = reinterpret_cast<char*>(&value);
        std::copy(buffer, buffer+sizeof(Value), dst);
        buffer += sizeof(Value);
        return value;
    }

    size_t size()
    {
        return std::distance(buffer_internal.begin(), buffer);
    }

    void reset()
    {
        buffer = std::begin(buffer_internal);
    }
};


typedef std::unique_ptr<Buffer> Buffer_sp;