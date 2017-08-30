#pragma once

#include <map>
#include <experimental/optional>
#include <vector>
#include "Common.h"
#include "Player.h"
#include "Tile.h"
#include "Train.h"


class Board {
public:
  Board(const std::vector<Player>& players);
  void newRound(int32 maxPips);
  uint32 poolSize() const;
  Tile dealTile();
  Train& getTrainById(id trainId);
  const Train& getTrainById(id trainId) const;

  std::map<id, Train> m_playerTrains;
  Train m_publicTrain;
  std::experimental::optional<Tile> m_centerTile;
  id m_centerPlaceId;

private:
  std::vector<Tile> m_tilePool;
  std::map<id, Train*> m_idToTrain;
};
