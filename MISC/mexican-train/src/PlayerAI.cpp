#include "PlayerAI.h"

#include "Board.h"
#include "Player.h"


PlayerAI::PlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board)
  : m_player(player),
  m_enemyPlayers(enemyPlayers),
  m_board(board) {
}

PlayerAI::~PlayerAI() {
}

void PlayerAI::message(const std::string& msg) {
}

void PlayerAI::notifyGameStart() {
}

void PlayerAI::notifyGameEnd() {
}

void PlayerAI::notifyRoundStart() {
}

void PlayerAI::notifyRoundEnd() {
}

void PlayerAI::notifyTilePlay(id playerId, id trainId, id tileId) {
}

void PlayerAI::notifyTileDraw(id playerId) {
}
