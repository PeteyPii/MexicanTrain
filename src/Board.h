#pragma once

#include <map>
#include <ostream>
#include <experimental/optional>
#include <vector>
#include "Common.h"
#include "GameSettings.h"
#include "Player.h"
#include "Tile.h"
#include "Train.h"


class Board {
public:
  Board(const std::vector<Player>& players, const GameSettings& gameSettings);
  void newRound();
  uint32 poolSize() const;
  Tile dealTile();
  Train& getTrainById(id trainId);
  const Train& getTrainById(id trainId) const;
  bool trainExists(id trainId) const;

  std::map<id, Train> m_playerTrains;
  std::experimental::optional<Tile> m_centerTile;
  id m_centerPlaceId;
  Train m_publicTrain;
  const GameSettings& m_gameSettings;

private:
  std::vector<Tile> m_tilePool;
  std::map<id, Train*> m_idToTrain;
};

std::ostream& operator<<(std::ostream& out, const Board& board);
