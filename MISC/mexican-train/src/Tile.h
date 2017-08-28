#pragma once

#include "Common.h"


class Tile {
public:
  Tile(int32 highPip, int32 lowPip);

  id m_id;
  int32 m_highPip;
  int32 m_lowPip;

private:
};
