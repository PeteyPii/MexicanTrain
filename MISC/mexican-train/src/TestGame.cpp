#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include "GameAdmin.h"
#include "GameSettings.h"
#include "RandomPlayerAI.h"


int main() {
  GameSettings gameSettings;
  gameSettings.m_maxPips = 6;
  gameSettings.m_startingHandSize = 9;
  gameSettings.m_numberOfPlayers = 1;

  GameAdmin gameAdmin;
  gameAdmin.runGame(gameSettings, [&gameSettings] (
    const std::vector<Player>& players,
    const std::vector<std::vector<EnemyPlayer>>& enemyPlayerLists,
    const Board& board
  ) -> std::vector<std::unique_ptr<PlayerAI>> {
    assert(players.size() == 1);
    assert(players.size() == enemyPlayerLists.size());

    std::vector<std::unique_ptr<PlayerAI>> playerAis;
    playerAis.emplace_back(std::make_unique<RandomPlayerAI>(players[0], enemyPlayerLists[0], board, gameSettings, &std::cout));
    return playerAis;
  });

  return 0;
}
