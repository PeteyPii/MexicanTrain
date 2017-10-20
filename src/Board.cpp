#include "Board.h"

#include <algorithm>
#include <cassert>
#include "IdentityGenerator.h"
#include "RNG.h"


Board::Board(const std::vector<Player>& players, const GameSettings& gameSettings) : m_centerPlaceId(IdentityGenerator::get().nextId()), m_gameSettings(gameSettings) {
  for (const auto& player : players) {
    m_playerTrains.insert(std::make_pair(player.m_id, Train()));
  }

  m_idToTrain.insert(std::make_pair(m_publicTrain.m_id, &m_publicTrain));
  for (auto& kv : m_playerTrains) {
    m_idToTrain.insert(std::make_pair(kv.second.m_id, &kv.second));
  }
}

void Board::newRound() {
  for (auto& kv : m_playerTrains) {
    kv.second.newRound();
  }
  m_tilePool.clear();
  m_publicTrain.newRound();
  m_publicTrain.m_isPublic = true;
  m_centerTile = {};

  for (int32 highPips = 0; highPips <= m_gameSettings.m_maxPips; highPips++) {
    for (int32 lowPips = 0; lowPips <= highPips; lowPips++) {
      m_tilePool.emplace_back(highPips, lowPips);
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

std::ostream& operator<<(std::ostream& out, const Board& board) {
  out << "Board:\n";
  out << "  Undrawn tiles: " << board.poolSize() << "\n";
  if (board.m_centerTile) {
    out << "  Center tile: {" << board.m_centerPlaceId << "} => " << *board.m_centerTile << "\n";
  } else {
    out << "  Center tile: {" << board.m_centerPlaceId << "} => NONE\n";
  }
  out << "  Public train: " << board.m_publicTrain << "\n";
  out << "  Player trains:\n";
  for (auto& it : board.m_playerTrains) {
    out << "    {" << it.first << "}'s => " << it.second << "\n";
  }
  return out;
}
