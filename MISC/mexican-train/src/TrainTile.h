#pragma once

#include <ostream>
#include "Tile.h"


class TrainTile {
public:
  TrainTile(Tile tile, bool isFlipped);

  Tile m_tile;
  bool m_isFlipped;  // flipped to low-high direction
};

std::ostream& operator<<(std::ostream& out, const TrainTile& trainTile);
