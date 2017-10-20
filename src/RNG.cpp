#include "RNG.h"

#include <chrono>
#include <iostream>

RNG& RNG::get() {
  static RNG instance;
  return instance;
}

RNG::RNG() {
  unsigned long long seed = std::chrono::system_clock::now().time_since_epoch().count();
  m_mt.seed(seed);
  std::cout << "RNG seed: " << seed << ".\n";
}
