#pragma once

#include <istream>
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
  HumanPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, std::istream& in, std::ostream* out = nullptr);
  virtual ~HumanPlayerAI();
  virtual TilePlay playTile() override;
  virtual void notifyTilePlay(id playerId, id placeId, id tileId) override;
  virtual void notifyGameResult(int32 placeFinished) override;
  virtual void message(const std::string& msg) override;

private:
  bool m_successfulPlay = true;
  bool m_makeRandomPlay = false;
  std::istream& m_in;
};
