#pragma once

#include <vector>
#include "Common.h"
#include "Tile.h"


class Train {
public:
  Train();
  void newRound();

  id m_id;
  std::vector<Tile> m_tiles;
  bool m_isPublic = false;

private:
};
