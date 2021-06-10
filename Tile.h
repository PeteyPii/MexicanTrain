#pragma once

#include "Common.h"
#include <ostream>

class Tile {
public:
  Tile(int32 highPip, int32 lowPip);

  id m_id;
  int32 m_highPips;
  int32 m_lowPips;

private:
};

std::ostream& operator<<(std::ostream& out, const Tile& tile);
