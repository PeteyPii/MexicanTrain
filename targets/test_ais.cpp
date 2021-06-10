#include "Game.h"
#include "GameSettingsWrapper.h"
#include "StatTracker.h"
#include "ai/GreedyPlayerAI.h"
#include "ai/HumanPlayerAI.h"
#include "ai/RandomPlayerAI.h"
#include "ai/SmartPlayerAI.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

using namespace std;

void testHumanAi() {
  GameSettingsWrapper gameSettings;
  gameSettings.setMaxPips(6);
  gameSettings.setStartingHandSize(9);
  gameSettings.setNumberOfPlayers(2);

  vector<unique_ptr<PlayerAI>> playerAis;
  playerAis.emplace_back(std::make_unique<HumanPlayerAI>(std::cin, &std::cout));
  playerAis.emplace_back(std::make_unique<RandomPlayerAI>(nullptr));
  Game Game(&std::cout, gameSettings, playerAis);
  Game.run();
}

void testRandomAi() {
  GameSettingsWrapper gameSettings;
  gameSettings.setMaxPips(6);
  gameSettings.setStartingHandSize(9);
  gameSettings.setNumberOfPlayers(2);

  for (int32 i = 0; i < 1000; i++) {
    vector<unique_ptr<PlayerAI>> playerAis;
    playerAis.emplace_back(std::make_unique<RandomPlayerAI>(nullptr));
    playerAis.emplace_back(std::make_unique<RandomPlayerAI>(nullptr));
    Game Game(nullptr, gameSettings, playerAis);
    Game.run();
  }
}

void testGreedyAi() {
  GameSettingsWrapper gameSettings;
  gameSettings.setMaxPips(6);
  gameSettings.setStartingHandSize(9);
  gameSettings.setNumberOfPlayers(2);

  for (int32 i = 0; i < 1000; i++) {
    vector<unique_ptr<PlayerAI>> playerAis;
    playerAis.emplace_back(std::make_unique<RandomPlayerAI>(nullptr));
    playerAis.emplace_back(std::make_unique<GreedyPlayerAI>(nullptr));
    Game Game(nullptr, gameSettings, playerAis);
    Game.run();
  }
}

void testSmartAi() {
  GameSettingsWrapper gameSettings;
  gameSettings.setMaxPips(12);
  gameSettings.setStartingHandSize(15);
  gameSettings.setNumberOfPlayers(3);

  for (int32 i = 0; i < 1000; i++) {
    vector<unique_ptr<PlayerAI>> playerAis;
    playerAis.emplace_back(std::make_unique<RandomPlayerAI>(nullptr));
    playerAis.emplace_back(std::make_unique<GreedyPlayerAI>(nullptr));
    playerAis.emplace_back(std::make_unique<SmartPlayerAI>(nullptr));
    Game Game(nullptr, gameSettings, playerAis);
    Game.run();
  }
}

int main() {
  testRandomAi();
  // testHumanAi();
  testGreedyAi();
  testSmartAi();

  std::cout << StatTracker::get();

  return 0;
}
