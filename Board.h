#pragma once

#include "Common.h"
#include "GameSettingsWrapper.h"
#include "Player.h"
#include "Tile.h"
#include "Train.h"
#include <map>
#include <optional>
#include <ostream>
#include <vector>

class Board {
public:
  Board(const std::vector<Player>& players, const GameSettingsWrapper& gameSettings);
  void newRound();
  uint32 poolSize() const;
  Tile dealTile();
  Train& getTrainById(id trainId);
  const Train& getTrainById(id trainId) const;
  bool trainExists(id trainId) const;

  std::map<id, Train> m_playerTrains;
  std::optional<Tile> m_centerTile;
  id m_centerPlaceId;
  Train m_publicTrain;
  const GameSettingsWrapper& m_gameSettings;

private:
  std::vector<Tile> m_tilePool;
  std::map<id, Train*> m_idToTrain;
};

std::ostream& operator<<(std::ostream& out, const Board& board);
