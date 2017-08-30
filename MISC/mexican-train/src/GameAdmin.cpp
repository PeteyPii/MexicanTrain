#include "GameAdmin.h"

#include <algorithm>
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
  cout << "Starting hand size: " << gameSettings.m_startingHandSize << endl;

  if (gameSettings.m_numberOfPlayers < 1 || gameSettings.m_numberOfPlayers > 8) {
    cerr << "There must be 1 - 8 players." << endl;
    return;
  }
  if (gameSettings.m_maxPips < 0) {
    cerr << "Max pip must be non-negative." << endl;
    return;
  }
  if (gameSettings.m_startingHandSize < 0 || gameSettings.m_startingHandSize > 25) {
    cerr << "Starting hand size must be between 0 and 25." << endl;
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
  for (int32 i = 0; i < (int32) players.size(); i++) {
    players[i].m_ai = playerAis[i].get();
    assert(&playerAis[i]->m_player == &players[i]);
  }

  for (auto& ai : playerAis) {
    ai->notifyGameStart();
  }

  std::vector<int32> incompleteRounds;
  for (int32 i = 0; i <= gameSettings.m_maxPips; i++) {
    incompleteRounds.push_back(i);
  }
  reverse(incompleteRounds.begin(), incompleteRounds.end());

  while (incompleteRounds.size() > 0) {
    board.newRound(gameSettings.m_maxPips);
    for (auto& player : players) {
      player.newRound();
    }

    for (auto& ai : playerAis) {
      ai->notifyRoundStart();
    }

    for (int32 i = 0; i < gameSettings.m_startingHandSize; i++) {
      for (auto& player : players) {
        Tile tile = board.dealTile();
        player.m_hand.push_back(tile);
        for (auto& ai : playerAis) {
          ai->notifyTileDraw(player.m_id);
        }
      }
    }

    auto playStartingTile = [&] () {
      while (true) {
        for (auto incompleteRoundIt = incompleteRounds.begin(); incompleteRoundIt != incompleteRounds.end(); incompleteRoundIt++) {
          for (auto& player : players) {
            for (auto tileIt = player.m_hand.begin(); tileIt != player.m_hand.end(); tileIt++) {
              if (*incompleteRoundIt == tileIt->m_highPip && *incompleteRoundIt == tileIt->m_lowPip) {
                while (true) {
                  TilePlay tilePlay = player.m_ai->playTile();
                  if (tilePlay.m_tileId == tileIt->m_id && tilePlay.m_placeId == board.m_centerPlaceId) {
                    board.m_centerTile = *tileIt;
                    player.m_hand.erase(tileIt);
                    incompleteRounds.erase(incompleteRoundIt);
                    for (auto& ai : playerAis) {
                      ai->notifyTilePlay(player.m_id, board.m_centerPlaceId, board.m_centerTile->m_id);
                    }
                    break;
                  }
                  player.m_ai->message("You must play the highest double that hasn't been intiated with this game.");
                }
                return;
              }
            }
          }
        }

        for (auto& player : players) {
          // TODO: Make leading player(s) draw more if the pool is not evenly divisible.
          if (board.poolSize() > 0) {
            Tile tile = board.dealTile();
            player.m_hand.push_back(tile);
            for (auto& ai : playerAis) {
              ai->notifyTileDraw(player.m_id);
            }
          }
        }
      }
    };
    playStartingTile();

    for (auto& ai : playerAis) {
      ai->notifyRoundEnd();
    }
  }

  for (auto& ai : playerAis) {
    ai->notifyGameEnd();
  }
}

