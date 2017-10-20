#include "GameAdmin.h"
#include "GameSettings.h"
#include "StatTracker.h"
#include "ai/GreedyPlayerAI.h"
#include "ai/HumanPlayerAI.h"
#include "ai/RandomPlayerAI.h"
#include "ai/SmartPlayerAI.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

int main() {
  {
    GameSettings gameSettings;
    gameSettings.m_maxPips = 6;
    gameSettings.m_startingHandSize = 9;
    gameSettings.m_numberOfPlayers = 2;

    GameAdmin gameAdmin(&std::cout);
    gameAdmin.runGame(
        gameSettings,
        [](const std::vector<Player>& players,
           const std::vector<std::vector<EnemyPlayer>>& enemyPlayerLists,
           const Board& board) -> std::vector<std::unique_ptr<PlayerAI>> {
          assert(players.size() == enemyPlayerLists.size());
          std::vector<std::unique_ptr<PlayerAI>> playerAis;
          playerAis.emplace_back(
              std::make_unique<HumanPlayerAI>(players[0], enemyPlayerLists[0], board, std::cin, &std::cout));
          playerAis.emplace_back(std::make_unique<RandomPlayerAI>(players[1], enemyPlayerLists[1], board, nullptr));
          return playerAis;
        });
  }

  {
      // GameSettings gameSettings;
      // gameSettings.m_maxPips = 6;
      // gameSettings.m_startingHandSize = 9;
      // gameSettings.m_numberOfPlayers = 2;

      // GameAdmin gameAdmin(nullptr);
      // for (int32 iters = 0; iters < 1000; iters++) {
      //   gameAdmin.runGame(gameSettings, [] (
      //     const std::vector<Player>& players,
      //     const std::vector<std::vector<EnemyPlayer>>& enemyPlayerLists,
      //     const Board& board
      //   ) -> std::vector<std::unique_ptr<PlayerAI>> {
      //     assert(players.size() == enemyPlayerLists.size());
      //     std::vector<std::unique_ptr<PlayerAI>> playerAis;
      //     for (int32 i = 0; i < (int32) players.size(); i++) {
      //       playerAis.emplace_back(std::make_unique<RandomPlayerAI>(players[i],
      //       enemyPlayerLists[i], board, nullptr));
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
      // for (int32 iters = 0; iters < 10000; iters++) {
      //   gameAdmin.runGame(gameSettings, [] (
      //     const std::vector<Player>& players,
      //     const std::vector<std::vector<EnemyPlayer>>& enemyPlayerLists,
      //     const Board& board
      //   ) -> std::vector<std::unique_ptr<PlayerAI>> {
      //     assert(players.size() == enemyPlayerLists.size());
      //     std::vector<std::unique_ptr<PlayerAI>> playerAis;
      //     for (int32 i = 0; i < (int32) players.size(); i++) {
      //       if (i % 2 == 0) {
      //         playerAis.emplace_back(std::make_unique<RandomPlayerAI>(players[i],
      //         enemyPlayerLists[i], board, nullptr));
      //       } else {
      //         playerAis.emplace_back(std::make_unique<GreedyPlayerAI>(players[i],
      //         enemyPlayerLists[i], board, nullptr));
      //       }
      //     }
      //     return playerAis;
      //   });
      // }
  }

  {
    // GameSettings gameSettings;
    // gameSettings.m_maxPips = 12;
    // gameSettings.m_startingHandSize = 15;
    // gameSettings.m_numberOfPlayers = 3;

    // GameAdmin gameAdmin(nullptr);
    // for (int32 iters = 0; iters < 10000; iters++) {
    //   gameAdmin.runGame(gameSettings, [] (
    //     const std::vector<Player>& players,
    //     const std::vector<std::vector<EnemyPlayer>>& enemyPlayerLists,
    //     const Board& board
    //   ) -> std::vector<std::unique_ptr<PlayerAI>> {
    //     assert(players.size() == enemyPlayerLists.size());
    //     std::vector<std::unique_ptr<PlayerAI>> playerAis;
    //     for (int32 i = 0; i < (int32) players.size(); i++) {
    //       if (i % 3 == 0) {
    //         playerAis.emplace_back(std::make_unique<RandomPlayerAI>(players[i],
    //         enemyPlayerLists[i], board, nullptr));
    //       } else if (i % 3 == 1) {
    //         playerAis.emplace_back(std::make_unique<GreedyPlayerAI>(players[i],
    //         enemyPlayerLists[i], board, nullptr));
    //       } else {
    //         playerAis.emplace_back(std::make_unique<SmartPlayerAI>(players[i],
    //         enemyPlayerLists[i], board, nullptr));
    //       }
    //     }
    //     return playerAis;
    //   });
    // }
  }

  std::cout << StatTracker::get();

  return 0;
}
