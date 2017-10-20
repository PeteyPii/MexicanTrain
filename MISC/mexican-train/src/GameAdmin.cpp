#include "GameAdmin.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include "Board.h"
#include "PlayerAI.h"
#include "RNG.h"
#include "Round.h"

using namespace std;


GameAdmin::GameAdmin(std::ostream* out) : m_out(out) {
}

void GameAdmin::runGame(
  const GameSettings& gameSettings,
  std::function<std::vector<std::unique_ptr<PlayerAI>>(
    const std::vector<Player>&,
    const std::vector<std::vector<EnemyPlayer>>&,
    const Board&
  )> aiCreator
) {
  if (m_out) {
    *m_out << "Number of players: " << gameSettings.m_numberOfPlayers << endl;
    *m_out << "Max domino pip count: " << gameSettings.m_maxPips << endl;
    *m_out << "Starting hand size: " << gameSettings.m_startingHandSize << endl;
  }

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
  Board board(players, gameSettings);

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
    board.newRound();
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
    Round round(gameSettings, board, players);

    playerTurn = round.playCenterTile(&incompleteRounds);
    lastActionTurn = playerTurn;

    bool roundOver = false;
    while (!roundOver) {
      Player& currPlayer = players[playerTurn];
      Train& currPlayerTrain = board.m_playerTrains[currPlayer.m_id];
      auto playTurn = [&] (const std::set<int32>& playablePips, const std::set<id>& playableTrains, const std::string& illegalPlayMessage) {
        if (round.playerHasPlay(currPlayer, playablePips)) {
          round.playTile(currPlayer, playableTrains, illegalPlayMessage, &activeDoubles, &activeDoublesTrainId, &roundOver);
          if (!activeDoubles) {
            lastActionTurn = playerTurn;
            playerTurn = (playerTurn + 1) % players.size();
          }
        } else {
          currPlayerTrain.m_isPublic = true;
          if (board.poolSize() > 0) {
            Tile tile = board.dealTile();
            currPlayer.m_hand.push_back(tile);
            for (auto& ai : playerAis) {
              ai->notifyTileDraw(currPlayer.m_id);
            }
            if (playablePips.count(tile.m_highPips) == 1 || playablePips.count(tile.m_lowPips) == 1) {
              currPlayerTrain.m_isPublic = false;
              round.playTile(currPlayer, playableTrains, illegalPlayMessage, &activeDoubles, &activeDoublesTrainId, &roundOver);
              if (!activeDoubles) {
                lastActionTurn = playerTurn;
                playerTurn = (playerTurn + 1) % players.size();
              }
            } else {
              lastActionTurn = playerTurn;
              playerTurn = (playerTurn + 1) % players.size();
            }
          } else if (lastActionTurn == playerTurn) {
            for (auto& ai : playerAis) {
              ai->notifyPassTurn(currPlayer.m_id);
            }
            roundOver = true;
          } else {
            playerTurn = (playerTurn + 1) % players.size();
            for (auto& ai : playerAis) {
              ai->notifyPassTurn(currPlayer.m_id);
            }
          }
        }
      };

      std::set<int32> playablePips;
      std::set<id> playableTrains;
      std::string illegalPlayMessage;
      if (activeDoubles) {
        Train& doublesTrain = board.getTrainById(activeDoublesTrainId);
        playablePips = {doublesTrain.m_tiles.back().m_tile.m_highPips};
        playableTrains = {activeDoublesTrainId};
        illegalPlayMessage = "You must play a valid tile from your hand onto the active doubles.";
        playTurn(playablePips, playableTrains, illegalPlayMessage);
      } else if (currPlayerTrain.m_tiles.size() > 0) {
        playablePips = round.standardPlayablePips(currPlayer);
        playableTrains = round.standardPlayableTrains(currPlayer);
        illegalPlayMessage = "You must make a valid play.";
        playTurn(playablePips, playableTrains, illegalPlayMessage);
      } else {
        playablePips = {board.m_centerTile->m_highPips};
        playableTrains = {currPlayerTrain.m_id};
        illegalPlayMessage = "You must play onto your own train first.";
        playTurn(playablePips, playableTrains, illegalPlayMessage);
      }
    }

    for (auto& player : players) {
      for (auto& tile : player.m_hand) {
        player.m_score += tile.m_highPips;
        player.m_score += tile.m_lowPips;
      }
    }

    for (auto& player : players) {
      if (player.m_hand.size() == 0) {
        player.m_roundsWon++;
      }
    }

    for (auto& ai : playerAis) {
      ai->notifyRoundEnd();
    }
  }

  for (auto& ai : playerAis) {
    ai->notifyGameEnd();
  }

  std::vector<int32> playerIndices;
  for (int32 i = 0; i < (int32) players.size(); i++) {
    playerIndices.push_back(i);
  }

  std::sort(playerIndices.begin(), playerIndices.end(), [&] (int32 p1, int32 p2) -> bool {
    if (players[p1].m_score < players[p2].m_score) {
      return true;
    } else if (players[p1].m_score == players[p2].m_score) {
      return players[p1].m_roundsWon > players[p2].m_roundsWon;
    } else {
      return false;
    }
  });
  auto startIt = playerIndices.begin();
  while (startIt != playerIndices.end()) {
    auto endIt = startIt + 1;
    while (endIt != playerIndices.end() && players[*endIt].m_score == players[*startIt].m_score && players[*endIt].m_roundsWon == players[*startIt].m_roundsWon) {
      endIt++;
    }
    std::shuffle(startIt, endIt, RNG::get().m_mt);
    startIt = endIt;
  }
  for (int32 i = 0; i < (int32) players.size(); i++) {
    players[playerIndices[i]].m_ai->notifyGameResult(i + 1);
  }
}

