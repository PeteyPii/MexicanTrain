#include "GameAdmin.h"

#include <iostream>
#include "PlayerAI.h"

using namespace std;



GameAdmin::GameAdmin() {
}

void GameAdmin::runGame(std::vector<PlayerAI*> playerAis, int32 maxPip) {
  cout << "Number of players: " << playerAis.size() << endl;
  cout << "Domino pip count: " << maxPip << endl;
}
