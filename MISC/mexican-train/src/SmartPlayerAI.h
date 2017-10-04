#pragma once

#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include "Common.h"
#include "EnemyPlayer.h"
#include "RandomPlayerAI.h"

class Board;
class Player;
class Train;


class SmartPlayerAI : public RandomPlayerAI {
public:
  SmartPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings, std::ostream* out = nullptr);
  virtual ~SmartPlayerAI();
  virtual TilePlay playTile() override;
  virtual void notifyTilePlay(id playerId, id trainId, id tileId) override;
  virtual void notifyGameResult(int32 placeFinished) override;
  virtual void message(const std::string& msg) override;

private:
  std::vector<id> m_plannedTrain;
  const Train& m_playerTrain;
  bool m_reevaluatePlays = true;
  bool m_successfulPlay = true;
};
