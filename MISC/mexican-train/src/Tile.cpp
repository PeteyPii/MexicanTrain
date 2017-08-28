#include "Tile.h"

#include <cassert>
#include "IdentityGenerator.h"


Tile::Tile(int32 highPip, int32 lowPip) : m_highPip(highPip), m_lowPip(lowPip) {
  assert(m_highPip >= m_lowPip);
  m_id = IdentityGenerator::get().nextId();
}
