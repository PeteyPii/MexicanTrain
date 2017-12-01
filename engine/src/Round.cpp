#include "Round.h"

#include "PlayerAI.h"
#include "RNG.h"
#include <algorithm>

Round::Round(
    GameSettingsWrapper& gameSettings, Board& board, std::vector<Player>& players, std::vector<int32>& incompleteRounds)
    : m_gameSettings(gameSettings), m_board(board), m_players(players), m_incompleteRounds(incompleteRounds) {
}

int32 Round::playCenterTile() {
  while (true) {
    for (auto incompleteRoundIt = m_incompleteRounds.begin(); incompleteRoundIt != m_incompleteRounds.end();
         incompleteRoundIt++) {
      for (auto playerIt = m_players.begin(); playerIt != m_players.end(); playerIt++) {
        for (auto tileIt = playerIt->m_hand.begin(); tileIt != playerIt->m_hand.end(); tileIt++) {
          if (*incompleteRoundIt == tileIt->m_highPips && *incompleteRoundIt == tileIt->m_lowPips) {
            while (true) {
              TilePlay tilePlay = playerIt->m_ai->playTile();
              auto playedTileIt =
                  find_if(playerIt->m_hand.begin(), playerIt->m_hand.end(), [&](const Tile& tile) -> bool {
                    return tile.m_id == tilePlay.m_tileId;
                  });
              bool validPlay = playedTileIt != playerIt->m_hand.end() &&
                  (playedTileIt->m_highPips == *incompleteRoundIt && playedTileIt->m_lowPips == *incompleteRoundIt) &&
                  tilePlay.m_placeId == m_board.m_centerPlaceId;
              if (validPlay) {
                m_board.m_centerTile = *playedTileIt;
                playerIt->m_hand.erase(playedTileIt);
                m_incompleteRounds.erase(incompleteRoundIt);
                for (auto& player : m_players) {
                  player.m_ai->notifyTilePlay(playerIt->m_id, m_board.m_centerPlaceId, m_board.m_centerTile->m_id);
                }
                break;
              }
              playerIt->m_ai->message("You must play the highest double that "
                                      "hasn't been initiated with this game.");
            }
            return playerIt - m_players.begin();
          }
        }
      }
    }

    if (m_board.poolSize() >= m_players.size()) {
      for (auto& player : m_players) {
        Tile tile = m_board.dealTile();
        player.m_hand.push_back(tile);
        for (auto& notifiee : m_players) {
          notifiee.m_ai->notifyTileDraw(player.m_id);
        }
      }
    } else {
      std::vector<int32> randomPlayerIndices;
      for (int i = 0; i < (int32)m_players.size(); i++) {
        randomPlayerIndices.push_back(i);
      }
      std::shuffle(randomPlayerIndices.begin(), randomPlayerIndices.end(), RNG::get().m_mt);
      for (int32 randomIndex : randomPlayerIndices) {
        if (m_board.poolSize() > 0) {
          Tile tile = m_board.dealTile();
          m_players[randomIndex].m_hand.push_back(tile);
          for (auto& notifiee : m_players) {
            notifiee.m_ai->notifyTileDraw(m_players[randomIndex].m_id);
          }
        }
      }
    }
  }
}

std::set<int32> Round::standardPlayablePips(Player& player) {
  std::set<int32> playablePips;
  for (auto& kv : m_board.m_playerTrains) {
    if (kv.first == player.m_id || kv.second.m_isPublic) {
      if (kv.second.m_tiles.size() > 0) {
        TrainTile& endTrainTile = kv.second.m_tiles.back();
        int32 trainPips = endTrainTile.m_isFlipped ? endTrainTile.m_tile.m_highPips : endTrainTile.m_tile.m_lowPips;
        playablePips.insert(trainPips);
      } else {
        playablePips.insert(m_board.m_centerTile->m_highPips);
      }
    }
  }
  if (m_board.m_publicTrain.m_tiles.size() > 0) {
    TrainTile& endTrainTile = m_board.m_publicTrain.m_tiles.back();
    int32 trainPips = endTrainTile.m_isFlipped ? endTrainTile.m_tile.m_highPips : endTrainTile.m_tile.m_lowPips;
    playablePips.insert(trainPips);
  } else {
    playablePips.insert(m_board.m_centerTile->m_highPips);
  }
  return playablePips;
}

std::set<id> Round::standardPlayableTrains(Player& player) {
  std::set<id> playableTrains;
  for (auto& kv : m_board.m_playerTrains) {
    if (kv.first == player.m_id || kv.second.m_isPublic) {
      playableTrains.insert(kv.second.m_id);
    }
  }
  playableTrains.insert(m_board.m_publicTrain.m_id);
  return playableTrains;
}

bool Round::playerHasPlay(Player& player, std::set<int32> playablePips) {
  for (auto& tile : player.m_hand) {
    if (playablePips.count(tile.m_highPips) > 0 || playablePips.count(tile.m_lowPips) > 0) {
      return true;
    }
  }
  return false;
}

void Round::playTile(Player& player, std::set<id> validTrainIds, const std::string& illegalPlayMessage) {
  while (true) {
    TilePlay tilePlay = player.m_ai->playTile();
    auto tileIt = find_if(player.m_hand.begin(), player.m_hand.end(), [&](const Tile& tile) -> bool {
      return tile.m_id == tilePlay.m_tileId;
    });
    bool validPlay = tileIt != player.m_hand.end() && validTrainIds.count(tilePlay.m_placeId) > 0;
    if (validPlay) {
      Train& targetTrain = m_board.getTrainById(tilePlay.m_placeId);
      int32 endPips = m_board.m_centerTile->m_highPips;
      if (targetTrain.m_tiles.size() > 0) {
        endPips = targetTrain.m_tiles.back().m_isFlipped ? targetTrain.m_tiles.back().m_tile.m_highPips
                                                         : targetTrain.m_tiles.back().m_tile.m_lowPips;
      }
      if (tileIt->m_highPips == endPips || tileIt->m_lowPips == endPips) {
        Tile playedTile = *tileIt;
        targetTrain.m_tiles.emplace_back(*tileIt, tileIt->m_highPips != endPips);
        player.m_hand.erase(tileIt);
        Train& playerTrain = m_board.m_playerTrains[player.m_id];
        if (tilePlay.m_placeId == playerTrain.m_id) {
          playerTrain.m_isPublic = false;
        }
        for (auto& notifiee : m_players) {
          notifiee.m_ai->notifyTilePlay(player.m_id, tilePlay.m_placeId, tilePlay.m_tileId);
        }
        m_areDoublesActive = false;
        m_activeDoublesTrainId = NULL_ID;
        if (player.m_hand.size() == 0) {
          m_isRoundOver = true;
        } else if (playedTile.m_highPips == playedTile.m_lowPips) {
          int32 countPlayed = 0;
          if (m_board.m_centerTile->m_highPips == playedTile.m_highPips) {
            countPlayed += 1;
          }
          for (auto& kv : m_board.m_playerTrains) {
            for (auto& trainTile : kv.second.m_tiles) {
              if (trainTile.m_tile.m_highPips == playedTile.m_highPips ||
                  trainTile.m_tile.m_lowPips == playedTile.m_highPips) {
                countPlayed += 1;
              }
            }
          }
          for (auto& trainTile : m_board.m_publicTrain.m_tiles) {
            if (trainTile.m_tile.m_highPips == playedTile.m_highPips ||
                trainTile.m_tile.m_lowPips == playedTile.m_highPips) {
              countPlayed += 1;
            }
          }
          if (countPlayed < m_gameSettings.maxPips() + 1) {
            m_areDoublesActive = true;
            m_activeDoublesTrainId = tilePlay.m_placeId;
          }
        }
        break;
      }
    }
    player.m_ai->message(illegalPlayMessage);
  }
}

void Round::run() {
  m_board.newRound();
  for (auto& player : m_players) {
    player.newRound();
  }

  for (auto& player : m_players) {
    player.m_ai->notifyRoundStart();
  }

  for (int32 i = 0; i < m_gameSettings.startingHandSize(); i++) {
    for (auto& player : m_players) {
      Tile tile = m_board.dealTile();
      player.m_hand.push_back(tile);
      for (auto& otherPlayer : m_players) {
        otherPlayer.m_ai->notifyTileDraw(player.m_id);
      }
    }
  }

  int32 playerTurn = -1;
  int32 lastActionTurn = -1;
  m_areDoublesActive = false;
  m_activeDoublesTrainId = NULL_ID;

  playerTurn = playCenterTile();
  lastActionTurn = playerTurn;

  m_isRoundOver = false;
  while (!m_isRoundOver) {
    Player& currPlayer = m_players[playerTurn];
    Train& currPlayerTrain = m_board.m_playerTrains[currPlayer.m_id];
    auto playTurn =
        [&](const std::set<int32>& playablePips,
            const std::set<id>& playableTrains,
            const std::string& illegalPlayMessage) {
          if (playerHasPlay(currPlayer, playablePips)) {
            playTile(currPlayer, playableTrains, illegalPlayMessage);
            if (!m_areDoublesActive) {
              lastActionTurn = playerTurn;
              playerTurn = (playerTurn + 1) % m_players.size();
            }
          } else {
            currPlayerTrain.m_isPublic = true;
            if (m_board.poolSize() > 0) {
              Tile tile = m_board.dealTile();
              currPlayer.m_hand.push_back(tile);
              for (auto& player : m_players) {
                player.m_ai->notifyTileDraw(currPlayer.m_id);
              }
              if (playablePips.count(tile.m_highPips) == 1 || playablePips.count(tile.m_lowPips) == 1) {
                currPlayerTrain.m_isPublic = false;
                playTile(currPlayer, playableTrains, illegalPlayMessage);
                if (!m_areDoublesActive) {
                  lastActionTurn = playerTurn;
                  playerTurn = (playerTurn + 1) % m_players.size();
                }
              } else {
                lastActionTurn = playerTurn;
                playerTurn = (playerTurn + 1) % m_players.size();
              }
            } else if (lastActionTurn == playerTurn) {
              for (auto& player : m_players) {
                player.m_ai->notifyPassTurn(currPlayer.m_id);
              }
              m_isRoundOver = true;
            } else {
              playerTurn = (playerTurn + 1) % m_players.size();
              for (auto& player : m_players) {
                player.m_ai->notifyPassTurn(currPlayer.m_id);
              }
            }
          }
        };

    std::set<int32> playablePips;
    std::set<id> playableTrains;
    std::string illegalPlayMessage;
    if (m_areDoublesActive) {
      Train& doublesTrain = m_board.getTrainById(m_activeDoublesTrainId);
      playablePips = {doublesTrain.m_tiles.back().m_tile.m_highPips};
      playableTrains = {m_activeDoublesTrainId};
      illegalPlayMessage = "You must play a valid tile from your hand onto "
                           "the active doubles.";
      playTurn(playablePips, playableTrains, illegalPlayMessage);
    } else if (currPlayerTrain.m_tiles.size() > 0) {
      playablePips = standardPlayablePips(currPlayer);
      playableTrains = standardPlayableTrains(currPlayer);
      illegalPlayMessage = "You must make a valid play.";
      playTurn(playablePips, playableTrains, illegalPlayMessage);
    } else {
      playablePips = {m_board.m_centerTile->m_highPips};
      playableTrains = {currPlayerTrain.m_id};
      illegalPlayMessage = "You must play onto your own train first.";
      playTurn(playablePips, playableTrains, illegalPlayMessage);
    }
  }

  for (auto& player : m_players) {
    for (auto& tile : player.m_hand) {
      player.m_score += tile.m_highPips;
      player.m_score += tile.m_lowPips;
    }
  }

  for (auto& player : m_players) {
    if (player.m_hand.size() == 0) {
      player.m_roundsWon++;
    }
  }

  for (auto& player : m_players) {
    player.m_ai->notifyRoundEnd();
  }
}
