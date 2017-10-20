#pragma once

#include <ostream>
#include <vector>
#include "Common.h"
#include "TrainTile.h"


class Train {
public:
  Train();
  void newRound();

  id m_id;
  std::vector<TrainTile> m_tiles;
  bool m_isPublic = false;
};

std::ostream& operator<<(std::ostream& out, const Train& train);
