#include "RandomPlayerAI.h"

#include "Board.h"
#include "RNG.h"


RandomPlayerAI::RandomPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings, std::ostream* out)
  : LoggingPlayerAI(player, enemyPlayers, board, gameSettings, out) {
  m_allPlaceIds.push_back(m_board.m_centerPlaceId);
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
