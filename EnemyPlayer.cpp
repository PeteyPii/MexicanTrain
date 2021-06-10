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

std::ostream& operator<<(std::ostream& out, const EnemyPlayer& player) {
  out << "Enemy Player {" << player.playerId() << "}\n";
  out << "  Points: " << player.score() << "\n";
  out << "  Rounds won: " << player.roundsWon() << "\n";
  out << "  Hand size: " << player.handSize() << "\n";
  return out;
}
