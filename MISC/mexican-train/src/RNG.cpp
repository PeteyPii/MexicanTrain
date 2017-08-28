#include "RNG.h"

#include <chrono>


RNG& RNG::get() {
  static RNG instance;
  return instance;
}

RNG::RNG() {
  m_mt.seed(std::chrono::system_clock::now().time_since_epoch().count());
}
