#include "GameAdmin.h"

#include <iostream>
#include "PlayerAI.h"

using namespace std;


GameAdmin::GameAdmin() {
}

void GameAdmin::runGame(
  int32 playerCount,
  int32 maxPip,
  std::function<
    std::vector<PlayerAI*>,
    const std::vector<Player>&,
    const std::vector<std::vector<EnemyPlayer>>&,
    const Board&
  > aiCreator
) {
  cout << "Number of players: " << playerCount << endl;
  cout << "Domino pip count: " << maxPip << endl;

  if (playerAis.size() < 1 || playerAis.size() > 8) {
    cerr << "There must be 1 - 8 players." << endl;
    return;
  }
  if (maxPip < 0) {
    cerr << "Max pip must be non-negative." << endl;
    return;
  }

  std::vector<Player> players(playerCount);
  std::vector<std::vector<EnemyPlayer>> enemyPlayers(playerCount);
  for (int32 i = 0; i < players.size(); i++) {
    for (int32 offset = 0; offset < players.size(); offset++) {

    }
  }

  Board board(players);


}

