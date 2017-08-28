#pragma once

#include <vector>
#include "Common.h"
#include "Tile.h"


class Player {
public:
  Player();

  id m_id;
  int32 m_score = 0;
  std::vector<Tile> m_hand;

private:
};
