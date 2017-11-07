#include "TrainTile.h"

TrainTile::TrainTile(Tile tile, bool isFlipped) : m_tile(tile), m_isFlipped(isFlipped) {
}

std::ostream& operator<<(std::ostream& out, const TrainTile& trainTile) {
  if (trainTile.m_isFlipped) {
    out << "[" << trainTile.m_tile.m_lowPips << ":" << trainTile.m_tile.m_highPips << "]";
  } else {
    out << "[" << trainTile.m_tile.m_highPips << ":" << trainTile.m_tile.m_lowPips << "]";
  }
  return out;
}
