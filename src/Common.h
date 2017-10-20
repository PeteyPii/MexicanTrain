#pragma once

#include <cstdint>
#include <sstream>
#include <string>

typedef int32_t int32;
typedef int64_t int64;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef uint64 id;

const id NULL_ID = 0;

template <typename T>
std::string toString(const T& t) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}
