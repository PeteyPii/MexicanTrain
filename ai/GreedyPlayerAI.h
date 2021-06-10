#pragma once

#include "Common.h"
#include "EnemyPlayer.h"
#include "ai/LoggingPlayerAI.h"
#include <ostream>
#include <string>
#include <vector>

class Board;
class Player;

class GreedyPlayerAI : public LoggingPlayerAI {
public:
  GreedyPlayerAI(std::ostream* out = nullptr);
  virtual ~GreedyPlayerAI();
  virtual void setUp(const Player* player, const std::vector<EnemyPlayer>* enemyPlayers, const Board* board) override;
  virtual TilePlay playTile() override;
  virtual void notifyTilePlay(id playerId, id placeId, id tileId) override;
  virtual void notifyGameResult(int32 placeFinished) override;
  virtual void message(const std::string& msg) override;

private:
  std::vector<id> m_allPlaceIds;
  std::vector<id> m_sortedTileIds;
  uint32 m_tileIndex = -1;
  uint32 m_placeIndex = -1;
  bool m_reevaluatePlays = true;
};
