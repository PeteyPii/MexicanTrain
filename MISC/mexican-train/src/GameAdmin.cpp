#include "GameAdmin.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
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

    int32 playerTurn = -1;
    int32 lastActionTurn = -1;
    bool activeDoubles = false;
    id activeDoublesTrainId = NULL_ID;
    auto playStartingTile = [&] () {
      while (true) {
        for (auto incompleteRoundIt = incompleteRounds.begin(); incompleteRoundIt != incompleteRounds.end(); incompleteRoundIt++) {
          for (auto playerIt = players.begin(); playerIt != players.end(); playerIt++) {
            for (auto tileIt = playerIt->m_hand.begin(); tileIt != playerIt->m_hand.end(); tileIt++) {
              if (*incompleteRoundIt == tileIt->m_highPips && *incompleteRoundIt == tileIt->m_lowPips) {
                while (true) {
                  TilePlay tilePlay = playerIt->m_ai->playTile();
                  if (tilePlay.m_tileId == tileIt->m_id && tilePlay.m_placeId == board.m_centerPlaceId) {
                    playerTurn = playerIt - players.begin();
                    lastActionTurn = playerTurn;
                    board.m_centerTile = *tileIt;
                    playerIt->m_hand.erase(tileIt);
                    incompleteRounds.erase(incompleteRoundIt);
                    for (auto& ai : playerAis) {
                      ai->notifyTilePlay(playerIt->m_id, board.m_centerPlaceId, board.m_centerTile->m_id);
                    }
                    break;
                  }
                  playerIt->m_ai->message("You must play the highest double that hasn't been intiated with this game.");
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

    bool roundOver = false;
    while (!roundOver) {
      Player& currPlayer = players[playerTurn];
      if (activeDoubles) {
        Train& doublesTrain = board.getTrainById(activeDoublesTrainId);
        int32 pips = doublesTrain.m_tiles.back().m_tile.m_highPips;
        bool hasPlay = false;
        for (auto& tile : currPlayer.m_hand) {
          if (tile.m_highPips == pips || tile.m_lowPips == pips) {
            hasPlay = true;
            break;
          }
        }

        if (hasPlay) {
          while (true) {
            TilePlay tilePlay = currPlayer.m_ai->playTile();
            auto tileIt = find_id(currPlayer.m_hand.begin(), currPlayer.m_hand.end(), [&](const Tile& tile) { return tile.m_id == tilePlay.m_tileId; });
            bool validPlay = tileIt != currPlayer.m_hand.end() &&
              (tileIt->m_highPips == pips || tileIt->m_lowPips == pips) &&
              tilePlay.m_placeId == activeDoublesTrainId;
            if (validPlay) {
              doublesTrain.m_tiles.emplace_back(*tileIt, tileIt->m_highPips != pips);
              currPlayer.m_hand.erase(tileIt);
              for (auto& ai : playerAis) {
                ai->notifyTilePlay(currPlayer.m_id, tilePlay->m_placeId, tilePlay->m_tileId);
              }
              lastActionTurn = playerTurn;
              if (currPlayer->m_hand.size() == 0) {
                roundOver = true;
              } else if () {  // TODO: ending point
              }
              break;
            }
            playerIt->m_ai->message("You must play a valid tile from your hand onto the active doubles.");
          }
        } else {
          if (board.poolSize() > 0) {
            Tile tile = board.dealTile();
            currPlayer.m_hand.push_back(tile);
            for (auto& ai : playerAis) {
              ai->notifyTileDraw(player.m_id);
            }
            lastActionTurn = playerTurn;
            playerTurn = (playerTurn + 1) % players.size();
          } else {
            if (lastActionTurn == playerTurn) {
              assert(false);  // this should not happen (the doubles would not have been set active then)
            } else {
              playerTurn = (playerTurn + 1) % players.size();
              for (auto& ai : playerAis) {
                ai->notifyPassTurn(currPlayer.m_id);
              }
            }
          }
        }
      } else if (board.m_playerTrains[currPlayer.m_id].m_tiles.size() > 0) {
        std::set<int32> playablePips;
        for (auto& kv : board.m_playerTrains) {
          if (kv.first == currPlayer.m_id || fv.second.m_isPublic) {
            if (kv.second.m_tiles.size() > 0) {
              TrainTile& endTrainTile = kv.second.m_tiles.back();
              int32 trainPips = endTrainTile.m_isFlipped ? endTrainTile.m_tile.m_lowPips : endTrainTile.m_tile.m_highPips;
              playablePips.insert(playablePips);
            } else {
              playablePips.insert(board.m_centerTile->m_highPips);
            }
          }
        }
        board.m_playerTrains[currPlayer.m_id]
      } else {
        bool hasPlay = false;
        for (auto& tile : currPlayer.m_hand) {
          if (tile.m_highPips == board.m_centerTile->m_highPips || tile
        }
      }
    }

    for (auto& ai : playerAis) {
      ai->notifyRoundEnd();
    }
  }

  for (auto& ai : playerAis) {
    ai->notifyGameEnd();
  }
}

