#pragma once

#include <vector>
#include "Common.h"
#include "Tile.h"


struct TrainTile {
  TrainTile(Tile tile, bool isFlipped) : m_tile(tile), m_isFlipped(isFlipped) {}

  Tile m_tile;
  bool m_isFlipped;  // flipped to low-high direction
};

class Train {
public:
  Train();
  void newRound();

  const id m_id;
  std::vector<TrainTile> m_tiles;
  bool m_isPublic = false;


private:
};
