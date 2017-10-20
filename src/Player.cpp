#include "Player.h"

#include "IdentityGenerator.h"


Player::Player() : m_id(IdentityGenerator::get().nextId()), m_enemyView(*this) {
}

void Player::newRound() {
  m_hand.clear();
}

std::ostream& operator<<(std::ostream& out, const Player& player) {
  out << "Player {" << player.m_id << "}\n";
  out << "  Points: " << player.m_score << "\n";
  out << "  Rounds won: " << player.m_roundsWon << "\n";
  out << "  Hand:";
  for (auto& tile : player.m_hand) {
    out << " {" << tile.m_id << "}=" << tile;
  }
  out << "\n";
  return out;
}
