#pragma once

#include <ostream>
#include "Common.h"

class Player;


class EnemyPlayer {
public:
  EnemyPlayer(const Player& player);
  id playerId() const;
  int32 score() const;
  int32 roundsWon() const;
  uint32 handSize() const;

private:
  const Player& m_player;
};

std::ostream& operator<<(std::ostream& out, const EnemyPlayer& enemyPlayer);
