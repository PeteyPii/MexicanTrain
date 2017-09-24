#pragma once

#include <ostream>
#include <string>
#include <vector>
#include "Common.h"
#include "EnemyPlayer.h"
#include "LoggingPlayerAI.h"

class Board;
class Player;


class GreedyPlayerAI : public LoggingPlayerAI {
public:
  GreedyPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings, std::ostream* out = nullptr);
  virtual ~GreedyPlayerAI();
  virtual TilePlay playTile() override;
  virtual void notifyTilePlay(id playerId, id trainId, id tileId) override;
  virtual void notifyGameResult(int32 placeFinished) override;
  virtual void message(const std::string& msg) override;

private:
  std::vector<id> m_allPlaceIds;
  std::vector<id> m_sortedTileIds;
  uint32 m_tileIndex = -1;
  uint32 m_placeIndex = -1;
  bool m_reevaluatePlays = true;
};
