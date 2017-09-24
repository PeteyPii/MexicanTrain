#include "GreedyPlayerAI.h"

#include <algorithm>
#include "Board.h"
#include "RNG.h"
#include "StatTracker.h"


GreedyPlayerAI::GreedyPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings, std::ostream* out)
  : LoggingPlayerAI(player, enemyPlayers, board, gameSettings, out) {
  for (const auto& kv : m_board.m_playerTrains) {
    m_allPlaceIds.push_back(kv.second.m_id);
  }
  m_allPlaceIds.push_back(m_board.m_publicTrain.m_id);
  m_allPlaceIds.push_back(m_board.m_centerPlaceId);
}

GreedyPlayerAI::~GreedyPlayerAI() {
}

TilePlay GreedyPlayerAI::playTile() {
  if (m_reevaluatePlays) {
    std::vector<int32> sortedIndices;
    for (int32 i = 0; i < (int32) m_player.m_hand.size(); i++) {
      sortedIndices.push_back(i);
    }
    std::sort(sortedIndices.begin(), sortedIndices.end(), [&] (int32 left, int32 right) -> bool {
      return m_player.m_hand[left].m_highPips + m_player.m_hand[left].m_lowPips > m_player.m_hand[right].m_highPips + m_player.m_hand[right].m_lowPips;
    });
    m_sortedTileIds.clear();
    for (int32 index : sortedIndices) {
      m_sortedTileIds.push_back(m_player.m_hand[index].m_id);
    }
    m_tileIndex = 0;
    m_placeIndex = 0;
    m_reevaluatePlays = false;
  }

  TilePlay tilePlay(m_sortedTileIds[m_tileIndex], m_allPlaceIds[m_placeIndex]);
  m_placeIndex++;
  if (m_placeIndex >= m_allPlaceIds.size()) {
    m_placeIndex = 0;
    m_tileIndex++;
  }
  return tilePlay;
}

void GreedyPlayerAI::notifyTilePlay(id playerId, id trainId, id tileId) {
  LoggingPlayerAI::notifyTilePlay(playerId, trainId, tileId);
  m_reevaluatePlays = true;
}

void GreedyPlayerAI::notifyGameResult(int32 placeFinished) {
  LoggingPlayerAI::notifyGameResult(placeFinished);
  int32 playerCount = 1 + m_enemyPlayers.size();
  Stats& stats = StatTracker::get().m_aiStats["GreedyPlayerAI"][playerCount];
  stats.m_finishPlaceCounts[placeFinished] += 1;
  stats.m_scores.push_back(m_player.m_score);
}

void GreedyPlayerAI::message(const std::string& msg) {
}

