#include "Tile.h"

#include "IdentityGenerator.h"
#include <cassert>

Tile::Tile(int32 highPip, int32 lowPip)
    : m_id(IdentityGenerator::get().nextId()), m_highPips(highPip), m_lowPips(lowPip) {
  assert(m_highPips >= m_lowPips);
}

std::ostream& operator<<(std::ostream& out, const Tile& tile) {
  return out << "[" << tile.m_highPips << ":" << tile.m_lowPips << "]";
}
