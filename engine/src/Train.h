#pragma once

#include "Common.h"
#include "TrainTile.h"
#include <ostream>
#include <vector>

class Train {
public:
  Train();
  void newRound();

  id m_id;
  std::vector<TrainTile> m_tiles;
  bool m_isPublic = false;
};

std::ostream& operator<<(std::ostream& out, const Train& train);
