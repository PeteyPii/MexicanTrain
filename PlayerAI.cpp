#include "PlayerAI.h"

#include "Board.h"
#include "Player.h"
#include "StatTracker.h"

PlayerAI::PlayerAI() {
}

PlayerAI::~PlayerAI() {
}

void PlayerAI::setUp(const Player* player, const std::vector<EnemyPlayer>* enemyPlayers, const Board* board) {
  m_player = player;
  m_enemyPlayers = enemyPlayers;
  m_board = board;
}

void PlayerAI::message(const std::string& msg) {
}

void PlayerAI::notifyGameStart() {
}

void PlayerAI::notifyGameEnd() {
}

void PlayerAI::notifyGameResult(int32 placeFinished) {
}

void PlayerAI::notifyRoundStart() {
}

void PlayerAI::notifyRoundEnd() {
}

void PlayerAI::notifyTilePlay(id playerId, id placeId, id tileId) {
}

void PlayerAI::notifyTileDraw(id playerId) {
}

void PlayerAI::notifyPassTurn(id playerId) {
}

void PlayerAI::trackGameResult(const std::string& aiKey, int32 placeFinished) {
  int32 playerCount = 1 + m_enemyPlayers->size();
  Stats& stats = StatTracker::get().m_aiStats[aiKey][playerCount];
  stats.m_finishPlaceCounts[placeFinished] += 1;
  stats.m_scores.push_back(m_player->m_score);
}
