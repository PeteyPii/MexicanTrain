#pragma once

#include <ostream>
#include <string>
#include <vector>
#include "Common.h"
#include "EnemyPlayer.h"
#include "LoggingPlayerAI.h"

class Board;
class Player;


class RandomPlayerAI : public LoggingPlayerAI {
public:
  RandomPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings, std::ostream* out = nullptr);
  virtual ~RandomPlayerAI();
  virtual TilePlay playTile() override;
  virtual void notifyGameResult(int32 placeFinished) override;
  virtual void message(const std::string& msg) override;

private:
  std::vector<id> m_allPlaceIds;
};
