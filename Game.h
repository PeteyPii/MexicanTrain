#pragma once

#include "Common.h"
#include "GameSettingsWrapper.h"
#include "Player.h"
#include <memory>
#include <vector>

class Board;
class PlayerAI;

class Game {
public:
  Game(std::ostream* out, const GameSettingsWrapper& gameSettings, std::vector<std::unique_ptr<PlayerAI>>& playerAis);
  void run();

private:
  std::ostream* m_out;
  GameSettingsWrapper m_gameSettings;
  std::vector<std::unique_ptr<PlayerAI>> m_playerAis;
  bool m_areSettingsValid = true;
};
