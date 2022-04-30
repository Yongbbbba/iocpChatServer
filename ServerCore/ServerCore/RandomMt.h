#pragma once
#include "pch.h"

#define RAND(type, maxVal) (type) RandomMT::getInstance().rand(maxVal)

class RandomMT : public Singleton<RandomMT> {
 public:
  uint64_t rand(int maxVal) {
    // MT19937 ���� ����
    std::mt19937 engine(
        (uint32_t)time(nullptr) +
        // (uint32_t)std:;this_thread::get_id().hash());
        (uint32_t)std::hash<std::thread::id>()(std::this_thread::get_id()));

    std::uniform_int_distribution<uint64_t> distribution(0, UINT64_MAX);
    // rand ���� �Լ������� bind
    auto generator = bind(distribution, engine);

    return (uint64_t)(generator() % maxVal);
  }
};
  


