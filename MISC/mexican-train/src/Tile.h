#pragma once

#include <ostream>
#include "Common.h"


class Tile {
public:
  Tile(int32 highPip, int32 lowPip);

  const id m_id;
  int32 m_highPips;
  int32 m_lowPips;

private:
};

std::ostream& operator<<(std::ostream& out, const Tile& tile);
