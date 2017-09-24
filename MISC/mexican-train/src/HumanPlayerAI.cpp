#include "HumanPlayerAI.h"

#include "Board.h"
#include "RNG.h"
#include "StatTracker.h"


HumanPlayerAI::HumanPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings, std::ostream* out)
  : RandomPlayerAI(player, enemyPlayers, board, gameSettings, out) {
}

HumanPlayerAI::~HumanPlayerAI() {
}

TilePlay HumanPlayerAI::playTile() {
  if (m_newPlay) {
    if (m_out) {
      *m_out << "\n";
      *m_out << m_board;
      for (auto& enemyPlayer : m_enemyPlayers) {
        *m_out << enemyPlayer;
      }
      *m_out << m_player;
      *m_out << "\n";
    }
    m_newPlay = false;
  }
  return RandomPlayerAI::playTile();
}

void HumanPlayerAI::notifyTilePlay(id playerId, id trainId, id tileId) {
  RandomPlayerAI::notifyTilePlay(playerId, trainId, tileId);
  m_newPlay = true;
}

void HumanPlayerAI::notifyGameResult(int32 placeFinished) {
  LoggingPlayerAI::notifyGameResult(placeFinished);
  int32 playerCount = 1 + m_enemyPlayers.size();
  Stats& stats = StatTracker::get().m_aiStats["HumanPlayerAI"][playerCount];
  stats.m_finishPlaceCounts[placeFinished] += 1;
  stats.m_scores.push_back(m_player.m_score);
}

void HumanPlayerAI::message(const std::string& msg) {
}
