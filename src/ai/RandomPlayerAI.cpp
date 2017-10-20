#include "RandomPlayerAI.h"

#include "Board.h"
#include "RNG.h"
#include "StatTracker.h"


RandomPlayerAI::RandomPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, std::ostream* out)
  : LoggingPlayerAI(player, enemyPlayers, board, out) {
  m_allPlaceIds.push_back(m_board.m_centerPlaceId);
  m_allPlaceIds.push_back(m_board.m_publicTrain.m_id);
  for (const auto& kv : m_board.m_playerTrains) {
    m_allPlaceIds.push_back(kv.second.m_id);
  }
}

RandomPlayerAI::~RandomPlayerAI() {
}

TilePlay RandomPlayerAI::playTile() {
  id tileId = m_player.m_hand[RNG::get().m_mt() % m_player.m_hand.size()].m_id;
  id placeId = m_allPlaceIds[RNG::get().m_mt() % m_allPlaceIds.size()];
  return TilePlay(tileId, placeId);
}

void RandomPlayerAI::notifyGameResult(int32 placeFinished) {
  LoggingPlayerAI::notifyGameResult(placeFinished);
  int32 playerCount = 1 + m_enemyPlayers.size();
  Stats& stats = StatTracker::get().m_aiStats["RandomPlayerAI"][playerCount];
  stats.m_finishPlaceCounts[placeFinished] += 1;
  stats.m_scores.push_back(m_player.m_score);
}

void RandomPlayerAI::message(const std::string& msg) {
}

