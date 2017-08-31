#include "Player.h"

#include "IdentityGenerator.h"


Player::Player() : m_id(IdentityGenerator::get().nextId()), m_enemyView(*this) {
}

void Player::newRound() {
  m_hand.clear();
}
