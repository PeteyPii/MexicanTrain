#include "Train.h"

#include "IdentityGenerator.h"


Train::Train() : m_id(IdentityGenerator::get().nextId()) {
}

void Train::newRound() {
  m_tiles.clear();
  m_isPublic = false;
}

std::ostream& operator<<(std::ostream& out, const Train& train) {
  out << "{" << train.m_id << "} " << (train.m_isPublic ? "(+)" : "( )") << " ";
  for (auto& tile : train.m_tiles) {
    out << tile;
  }
  return out;
}
