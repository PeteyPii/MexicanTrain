#pragma once

#include <random>

class RNG {
public:
  static RNG& get();

  RNG(const RNG& other) = delete;
  RNG& operator=(const RNG& other) = delete;

  std::mt19937 m_mt;

private:
  RNG();
};
