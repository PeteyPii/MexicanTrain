#pragma once

#include "Common.h"
#include "EnemyPlayer.h"
#include "ai/LoggingPlayerAI.h"
#include <ostream>
#include <string>
#include <vector>

class Board;
class Player;

class RandomPlayerAI : public LoggingPlayerAI {
public:
  RandomPlayerAI(std::ostream* out = nullptr);
  virtual ~RandomPlayerAI();
  virtual void setUp(const Player* player, const std::vector<EnemyPlayer>* enemyPlayers, const Board* board) override;
  virtual TilePlay playTile() override;
  virtual void notifyGameResult(int32 placeFinished) override;
  virtual void message(const std::string& msg) override;

private:
  std::vector<id> m_allPlaceIds;
};
