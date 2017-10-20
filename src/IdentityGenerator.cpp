#include "IdentityGenerator.h"


IdentityGenerator& IdentityGenerator::get() {
  static IdentityGenerator instance;
  return instance;
}

id IdentityGenerator::nextId() {
  return m_nextId++;
}

IdentityGenerator::IdentityGenerator() {
}
