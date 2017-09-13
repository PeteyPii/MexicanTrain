#include "Board.h"

#include <algorithm>
#include <cassert>
#include "IdentityGenerator.h"
#include "RNG.h"


Board::Board(const std::vector<Player>& players) {
  for (const auto& player : players) {
    m_playerTrains.insert(std::make_pair(player.m_id, Train()));
  }

  m_idToTrain.insert(std::make_pair(m_publicTrain.m_id, &m_publicTrain));
  for (auto& kv : m_playerTrains) {
    m_idToTrain.insert(std::make_pair(kv.second.m_id, &kv.second));
  }
  m_centerPlaceId = IdentityGenerator::get().nextId();
}

void Board::newRound(int32 maxPips) {
  assert(maxPips >= 0);

  for (auto& kv : m_playerTrains) {
    kv.second.newRound();
  }
  m_tilePool.clear();
  m_publicTrain.newRound();
  m_publicTrain.m_isPublic = true;
  m_centerTile = {};

  for (int32 highPip = 0; highPip <= maxPips; highPip++) {
    for (int32 lowPip = 0; lowPip <= highPip; lowPip++) {
      m_tilePool.emplace_back(highPip, lowPip);
    }
  }

  std::shuffle(m_tilePool.begin(), m_tilePool.end(), RNG::get().m_mt);
}

uint32 Board::poolSize() const {
  return m_tilePool.size();
}

Tile Board::dealTile() {
  Tile dealtTile = m_tilePool.back();
  m_tilePool.pop_back();
  return dealtTile;
}

Train& Board::getTrainById(id trainId) {
  auto it = m_idToTrain.find(trainId);
  assert(it != m_idToTrain.end());
  return *it->second;
}

const Train& Board::getTrainById(id trainId) const {
  auto it = m_idToTrain.find(trainId);
  assert(it != m_idToTrain.end());
  return *it->second;
}

bool Board::trainExists(id trainId) const {
  return m_idToTrain.count(trainId) > 0;
}
