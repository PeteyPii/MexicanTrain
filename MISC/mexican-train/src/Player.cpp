#include "Player.h"

#include "IdentityGenerator.h"


Player::Player() : m_enemyView(*this) {
  m_id = IdentityGenerator::get().nextId();
}

void Player::newRound() {
  m_hand.clear();
}
