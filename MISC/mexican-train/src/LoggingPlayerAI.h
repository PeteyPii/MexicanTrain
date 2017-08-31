#pragma once

#include <ostream>
#include <string>
#include <vector>
#include "Common.h"
#include "EnemyPlayer.h"
#include "PlayerAI.h"

class Board;
class Player;


class LoggingPlayerAI : public PlayerAI {
public:
  LoggingPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings, std::ostream* out = nullptr);
  virtual ~LoggingPlayerAI();
  virtual TilePlay playTile() override = 0;
  virtual void message(const std::string& msg) override;
  virtual void notifyGameStart() override;
  virtual void notifyGameEnd() override;
  virtual void notifyRoundStart() override;
  virtual void notifyRoundEnd() override;
  virtual void notifyTilePlay(id playerId, id trainId, id tileId) override;
  virtual void notifyTileDraw(id playerId) override;
  virtual void notifyPassTurn(id playerId) override;

protected:
  std::ostream* m_out;

private:
  std::string messagePrefix();
};
