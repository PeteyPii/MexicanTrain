#pragma once

#include "Common.h"

class Player;


class EnemyPlayer {
public:
  EnemyPlayer(const Player& player);
  id playerId() const;
  int32 score() const;
  uint32 handSize() const;

private:
  const Player& m_player;
};
