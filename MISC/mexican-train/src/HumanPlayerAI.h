#pragma once

#include <ostream>
#include <string>
#include <vector>
#include "Common.h"
#include "EnemyPlayer.h"
#include "RandomPlayerAI.h"

class Board;
class Player;


class HumanPlayerAI : public RandomPlayerAI {
public:
  HumanPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings, std::ostream* out = nullptr);
  virtual ~HumanPlayerAI();
  virtual TilePlay playTile() override;
  virtual void notifyTilePlay(id playerId, id trainId, id tileId) override;
  virtual void notifyGameResult(int32 placeFinished) override;
  virtual void message(const std::string& msg) override;

private:
  bool m_newPlay = true;
};
