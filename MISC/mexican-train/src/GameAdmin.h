#pragma once

#include <vector>
#include "Common.h"

class PlayerAI;


class GameAdmin {
public:
  GameAdmin();
  void runGame(std::vector<PlayerAI*> playerAis, int32 maxPip);

private:
};
