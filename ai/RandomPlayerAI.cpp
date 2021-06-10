#include "RandomPlayerAI.h"

#include "Board.h"
#include "RNG.h"
#include "StatTracker.h"

RandomPlayerAI::RandomPlayerAI(std::ostream* out) : LoggingPlayerAI(out) {
}

void RandomPlayerAI::setUp(const Player* player, const std::vector<EnemyPlayer>* enemyPlayers, const Board* board) {
  PlayerAI::setUp(player, enemyPlayers, board);
  m_allPlaceIds.push_back(m_board->m_centerPlaceId);
  m_allPlaceIds.push_back(m_board->m_publicTrain.m_id);
  for (const auto& kv : m_board->m_playerTrains) {
    m_allPlaceIds.push_back(kv.second.m_id);
  }
}

RandomPlayerAI::~RandomPlayerAI() {
}

TilePlay RandomPlayerAI::playTile() {
  id tileId = m_player->m_hand[RNG::get().m_mt() % m_player->m_hand.size()].m_id;
  id placeId = m_allPlaceIds[RNG::get().m_mt() % m_allPlaceIds.size()];
  return TilePlay(tileId, placeId);
}

void RandomPlayerAI::notifyGameResult(int32 placeFinished) {
  LoggingPlayerAI::notifyGameResult(placeFinished);
  trackGameResult("RandomPlayerAI", placeFinished);
}

void RandomPlayerAI::message(const std::string& msg) {
}
