#include "RandomPlayerAI.h"

RandomPlayerAI::RandomPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings, std::ostream* out)
  : LoggingPlayerAI(player, enemyPlayers, board, gameSettings, out) {
}

RandomPlayerAI::~RandomPlayerAI() {
}

TilePlay RandomPlayerAI::playTile() {
  return TilePlay(0, 0);
}
