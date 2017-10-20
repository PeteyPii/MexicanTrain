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
