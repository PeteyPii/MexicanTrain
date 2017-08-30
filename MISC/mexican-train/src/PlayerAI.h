#pragma once

#include <string>
#include <vector>
#include "Common.h"
#include "EnemyPlayer.h"
#include "GameSettings.h"

class Board;
class Player;


struct TilePlay {
  TilePlay(id tileId, id trainId) : m_tileId(tileId), m_trainId(trainId) {}
  id m_tileId;
  id m_trainId;
};

class PlayerAI {
public:
  PlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings);
  virtual ~PlayerAI();
  virtual TilePlay playTile() = 0;
  virtual void message(const std::string& msg);
  virtual void notifyGameStart();
  virtual void notifyGameEnd();
  virtual void notifyRoundStart();
  virtual void notifyRoundEnd();
  virtual void notifyTilePlay(id playerId, id trainId, id tileId);
  virtual void notifyTileDraw(id playerId);

protected:
  const Player& m_player;
  const std::vector<EnemyPlayer>& m_enemyPlayers;
  const Board& m_board;
  const GameSettings& m_gameSettings;
};
