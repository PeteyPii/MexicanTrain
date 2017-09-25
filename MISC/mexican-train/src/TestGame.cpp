#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include "GameAdmin.h"
#include "GameSettings.h"
#include "GreedyPlayerAI.h"
#include "HumanPlayerAI.h"
#include "RandomPlayerAI.h"
#include "StatTracker.h"

using namespace std;


int main() {
  {
    GameSettings gameSettings;
    gameSettings.m_maxPips = 6;
    gameSettings.m_startingHandSize = 9;
    gameSettings.m_numberOfPlayers = 2;

    GameAdmin gameAdmin(&std::cout);
    gameAdmin.runGame(gameSettings, [&gameSettings] (
      const std::vector<Player>& players,
      const std::vector<std::vector<EnemyPlayer>>& enemyPlayerLists,
      const Board& board
    ) -> std::vector<std::unique_ptr<PlayerAI>> {
      assert(players.size() == enemyPlayerLists.size());
      std::vector<std::unique_ptr<PlayerAI>> playerAis;
      playerAis.emplace_back(std::make_unique<HumanPlayerAI>(players[0], enemyPlayerLists[0], board, gameSettings, &std::cout));
      playerAis.emplace_back(std::make_unique<RandomPlayerAI>(players[1], enemyPlayerLists[1], board, gameSettings, nullptr));
      return playerAis;
    });
  }

  {
    // GameSettings gameSettings;
    // gameSettings.m_maxPips = 6;
    // gameSettings.m_startingHandSize = 9;
    // gameSettings.m_numberOfPlayers = 2;

    // GameAdmin gameAdmin(nullptr);
    // for (int32 i = 0; i < 1000; i++) {
    //   gameAdmin.runGame(gameSettings, [&gameSettings] (
    //     const std::vector<Player>& players,
    //     const std::vector<std::vector<EnemyPlayer>>& enemyPlayerLists,
    //     const Board& board
    //   ) -> std::vector<std::unique_ptr<PlayerAI>> {
    //     assert(players.size() == enemyPlayerLists.size());
    //     std::vector<std::unique_ptr<PlayerAI>> playerAis;
    //     for (int32 i = 0; i < (int32) players.size(); i++) {
    //       playerAis.emplace_back(std::make_unique<RandomPlayerAI>(players[i], enemyPlayerLists[i], board, gameSettings, nullptr));
    //     }
    //     return playerAis;
    //   });
    // }
  }

  {
    // GameSettings gameSettings;
    // gameSettings.m_maxPips = 6;
    // gameSettings.m_startingHandSize = 9;
    // gameSettings.m_numberOfPlayers = 2;

    // GameAdmin gameAdmin(nullptr);
    // for (int32 i = 0; i < 10000; i++) {
    //   gameAdmin.runGame(gameSettings, [&gameSettings] (
    //     const std::vector<Player>& players,
    //     const std::vector<std::vector<EnemyPlayer>>& enemyPlayerLists,
    //     const Board& board
    //   ) -> std::vector<std::unique_ptr<PlayerAI>> {
    //     assert(players.size() == enemyPlayerLists.size());
    //     std::vector<std::unique_ptr<PlayerAI>> playerAis;
    //     for (int32 i = 0; i < (int32) players.size(); i++) {
    //       if (i % 2 == 0) {
    //         playerAis.emplace_back(std::make_unique<RandomPlayerAI>(players[i], enemyPlayerLists[i], board, gameSettings, nullptr));
    //       } else {
    //         playerAis.emplace_back(std::make_unique<GreedyPlayerAI>(players[i], enemyPlayerLists[i], board, gameSettings, nullptr));
    //       }
    //     }
    //     return playerAis;
    //   });
    // }
  }

  std::cout << StatTracker::get();

  return 0;
}
