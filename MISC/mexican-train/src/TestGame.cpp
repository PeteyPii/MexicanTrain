#include <iostream>
#include <vector>
#include "GameAdmin.h"
#include "PlayerAI.h"


int main() {
  GameAdmin gameAdmin;
  gameAdmin.runGame(std::vector<PlayerAI*>(), 15);

  return 0;
}
