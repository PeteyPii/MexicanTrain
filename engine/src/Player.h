#pragma once

#include "Common.h"
#include "EnemyPlayer.h"
#include "Tile.h"
#include <ostream>
#include <vector>

class PlayerAI;

class Player {
public:
  Player();
  void newRound();

  id m_id;
  int32 m_score = 0;
  int32 m_roundsWon = 0;
  std::vector<Tile> m_hand;
  const EnemyPlayer m_enemyView;
  PlayerAI* m_ai = nullptr;
};

std::ostream& operator<<(std::ostream& out, const Player& player);
