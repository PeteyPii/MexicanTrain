#pragma once

#include <map>
#include <vector>
#include "Common.h"
#include "Player.h"
#include "Tile.h"
#include "Train.h"


// Represents all the dominos on the table.
class Board {
public:
  Board(const std::vector<Player>& players);
  void newRound(int32 maxPip);
  uint32 poolSize() const;
  Tile dealTile();
  Train& getTrainById(id trainId);
  const Train& getTrainById(id trainId) const;

  std::map<id, Train> m_playerTrains;
  Train m_publicTrain;

private:
  std::vector<Tile> m_tilePool;
  std::map<id, Train*> m_idToTrain;
};
