#pragma once

#include "Common.h"

class IdentityGenerator {
public:
  static IdentityGenerator& get();

  IdentityGenerator(const IdentityGenerator& other) = delete;
  IdentityGenerator& operator=(const IdentityGenerator& other) = delete;
  id nextId();

private:
  IdentityGenerator();

  id m_nextId = NULL_ID + 1;
};
