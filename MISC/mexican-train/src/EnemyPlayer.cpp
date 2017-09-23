#include "EnemyPlayer.h"

#include "Player.h"


EnemyPlayer::EnemyPlayer(const Player& player) : m_player(player) {
}

id EnemyPlayer::playerId() const {
  return m_player.m_id;
}

int32 EnemyPlayer::score() const {
  return m_player.m_score;
}

int32 EnemyPlayer::roundsWon() const {
  return m_player.m_roundsWon;
}

uint32 EnemyPlayer::handSize() const {
  return m_player.m_hand.size();
}
