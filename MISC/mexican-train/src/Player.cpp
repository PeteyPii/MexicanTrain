#include "Player.h"

#include "IdentityGenerator.h"


Player::Player() {
  m_id = IdentityGenerator::get().nextId();
}
