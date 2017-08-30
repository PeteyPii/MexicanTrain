#pragma once

#include <functional>
#include <memory>
#include <vector>
#include "Common.h"
#include "GameSettings.h"
#include "Player.h"

class Board;
class PlayerAI;


class GameAdmin {
public:
  GameAdmin();
  void runGame(
    const GameSettings& gameSettings,
    std::function<std::vector<std::unique_ptr<PlayerAI>>(
      const std::vector<Player>&,
      const std::vector<std::vector<EnemyPlayer>>&,
      const Board&
    )> aiCreator
  );

private:
};
