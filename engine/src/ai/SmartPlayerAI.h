#pragma once

#include "Common.h"
#include "EnemyPlayer.h"
#include "ai/RandomPlayerAI.h"
#include <ostream>
#include <string>
#include <utility>
#include <vector>

class Board;
class Player;
class Train;

class SmartPlayerAI : public RandomPlayerAI {
public:
  SmartPlayerAI(std::ostream* out = nullptr);
  virtual ~SmartPlayerAI();
  virtual void setUp(const Player* player, const std::vector<EnemyPlayer>* enemyPlayers, const Board* board) override;
  virtual TilePlay playTile() override;
  virtual void notifyRoundStart();
  virtual void notifyTilePlay(id playerId, id placeId, id tileId) override;
  virtual void notifyTileDraw(id playerId) override;
  virtual void notifyPassTurn(id playerId) override;
  virtual void notifyGameResult(int32 placeFinished) override;
  virtual void message(const std::string& msg) override;

private:
  std::vector<id> m_plannedTrain;
  std::vector<id> m_spareTiles;
  const Train* m_playerTrain = nullptr;
  std::vector<const Train*> m_otherTrains;
  bool m_reevaluatePlays = true;
  bool m_tryingLockPlay = false;
  bool m_tryingSparePlay = false;
  bool m_tryingTrainPlay = false;
  bool m_fallbackRandomPlay = false;
  uint32 m_spareTrainIndex;
  uint32 m_spareTileIndex;
};
