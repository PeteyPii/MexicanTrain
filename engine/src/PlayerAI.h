#pragma once

#include "Common.h"
#include "EnemyPlayer.h"
#include <string>
#include <vector>

class Board;
class Player;

struct TilePlay {
  TilePlay(id tileId, id placeId) : m_tileId(tileId), m_placeId(placeId) {
  }
  id m_tileId;
  id m_placeId;
};

class PlayerAI {
public:
  PlayerAI();
  virtual ~PlayerAI();
  virtual void setUp(const Player* player, const std::vector<EnemyPlayer>* enemyPlayers, const Board* board);
  virtual TilePlay playTile() = 0;
  virtual void message(const std::string& msg);
  virtual void notifyGameStart();
  virtual void notifyGameEnd();
  virtual void notifyGameResult(int32 placeFinished);
  virtual void notifyRoundStart();
  virtual void notifyRoundEnd();
  virtual void notifyTilePlay(id playerId, id placeId, id tileId);
  virtual void notifyTileDraw(id playerId);
  virtual void notifyPassTurn(id playerId);
  void trackGameResult(const std::string& aiKey, int32 placeFinished);

  const Player* m_player = nullptr;

protected:
  const std::vector<EnemyPlayer>* m_enemyPlayers = nullptr;
  const Board* m_board = nullptr;
};
