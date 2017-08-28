#include "Train.h"

#include "IdentityGenerator.h"


Train::Train() {
  m_id = IdentityGenerator::get().nextId();
}

void Train::newRound() {
  m_tiles.clear();
  m_isPublic = false;
}
