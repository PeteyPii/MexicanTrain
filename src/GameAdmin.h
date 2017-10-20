#pragma once

#include "Common.h"
#include "GameSettings.h"
#include "Player.h"
#include <functional>
#include <memory>
#include <vector>

class Board;
class PlayerAI;

class GameAdmin {
public:
  GameAdmin(std::ostream* out);
  void runGame(
      const GameSettings& gameSettings,
      std::function<std::vector<std::unique_ptr<PlayerAI>>(
          const std::vector<Player>&, const std::vector<std::vector<EnemyPlayer>>&, const Board&)> aiCreator);

private:
  std::ostream* m_out;
};
