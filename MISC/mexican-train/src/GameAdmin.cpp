#include "GameAdmin.h"

#include <cassert>
#include <iostream>
#include "Board.h"
#include "PlayerAI.h"

using namespace std;


GameAdmin::GameAdmin() {
}

void GameAdmin::runGame(
  const GameSettings& gameSettings,
  std::function<std::vector<std::unique_ptr<PlayerAI>>(
    const std::vector<Player>&,
    const std::vector<std::vector<EnemyPlayer>>&,
    const Board&
  )> aiCreator
) {
  cout << "Number of players: " << gameSettings.m_numberOfPlayers << endl;
  cout << "Max domino pip count: " << gameSettings.m_maxPips << endl;

  if (gameSettings.m_numberOfPlayers < 1 || gameSettings.m_numberOfPlayers > 8) {
    cerr << "There must be 1 - 8 players." << endl;
    return;
  }
  if (gameSettings.m_maxPips < 0) {
    cerr << "Max pip must be non-negative." << endl;
    return;
  }

  std::vector<Player> players(gameSettings.m_numberOfPlayers);
  std::vector<std::vector<EnemyPlayer>> enemyPlayerLists(gameSettings.m_numberOfPlayers);
  for (int32 i = 0; i < (int32) players.size(); i++) {
    for (int32 offset = 1; offset < (int32) players.size(); offset++) {
      enemyPlayerLists[i].push_back(players[(i + offset) % players.size()].m_enemyView);
    }
  }
  Board board(players);

  std::vector<std::unique_ptr<PlayerAI>> playerAis(aiCreator(players, enemyPlayerLists, board));

  assert((int32) playerAis.size() == gameSettings.m_numberOfPlayers);

  for (auto& ai : playerAis) {
    ai->notifyGameStart();
  }

  for (auto& ai : playerAis) {
    ai->notifyGameEnd();
  }
}

