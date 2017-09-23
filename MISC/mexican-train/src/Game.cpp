#include "Game.h"

#include <algorithm>
#include "PlayerAI.h"

Game::Game(GameSettings gameSettings, Board& board, std::vector<Player>& players)
  : m_gameSettings(gameSettings),
  m_board(board),
  m_players(players) {
}

int32 Game::playCenterTile(std::vector<int32>* incompleteRounds) {
  while (true) {
    for (auto incompleteRoundIt = incompleteRounds->begin(); incompleteRoundIt != incompleteRounds->end(); incompleteRoundIt++) {
      for (auto playerIt = m_players.begin(); playerIt != m_players.end(); playerIt++) {
        for (auto tileIt = playerIt->m_hand.begin(); tileIt != playerIt->m_hand.end(); tileIt++) {
          if (*incompleteRoundIt == tileIt->m_highPips && *incompleteRoundIt == tileIt->m_lowPips) {
            while (true) {
              TilePlay tilePlay = playerIt->m_ai->playTile();
              auto playedTileIt = find_if(playerIt->m_hand.begin(), playerIt->m_hand.end(), [&] (const Tile& tile) { return tile.m_id == tilePlay.m_tileId; });
              bool validPlay = playedTileIt != playerIt->m_hand.end() &&
                (playedTileIt->m_highPips == *incompleteRoundIt && playedTileIt->m_lowPips == *incompleteRoundIt) &&
                tilePlay.m_placeId == m_board.m_centerPlaceId;
              if (validPlay) {
                m_board.m_centerTile = *playedTileIt;
                playerIt->m_hand.erase(playedTileIt);
                incompleteRounds->erase(incompleteRoundIt);
                for (auto& player : m_players) {
                  player.m_ai->notifyTilePlay(playerIt->m_id, m_board.m_centerPlaceId, m_board.m_centerTile->m_id);
                }
                break;
              }
              playerIt->m_ai->message("You must play the highest double that hasn't been initiated with this game.");
            }
            return playerIt - m_players.begin();
          }
        }
      }
    }

    for (auto& player : m_players) {
      // TODO: Make leading player(s) draw more if the pool is not evenly divisible.
      if (m_board.poolSize() > 0) {
        Tile tile = m_board.dealTile();
        player.m_hand.push_back(tile);
        for (auto& notifiee : m_players) {
          notifiee.m_ai->notifyTileDraw(player.m_id);
        }
      }
    }
  }
}

std::set<int32> Game::standardPlayablePips(Player& player) {
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

std::set<id> Game::standardPlayableTrains(Player& player) {
  std::set<id> playableTrains;
  for (auto& kv : m_board.m_playerTrains) {
    if (kv.first == player.m_id || kv.second.m_isPublic) {
      playableTrains.insert(kv.second.m_id);
    }
  }
  playableTrains.insert(m_board.m_publicTrain.m_id);
  return playableTrains;
}

bool Game::playerHasPlay(Player& player, std::set<int32> playablePips) {
  for (auto& tile : player.m_hand) {
    if (playablePips.count(tile.m_highPips) > 0 || playablePips.count(tile.m_lowPips) > 0) {
      return true;
    }
  }
  return false;
}

void Game::playTile(Player& player, std::set<id> validTrainIds, const std::string& illegalPlayMessage, bool* activeDoubles, id* activeDoublesTrainId, bool* roundOver) {
  while (true) {
    TilePlay tilePlay = player.m_ai->playTile();
    auto tileIt = find_if(player.m_hand.begin(), player.m_hand.end(), [&] (const Tile& tile) { return tile.m_id == tilePlay.m_tileId; });
    bool validPlay = tileIt != player.m_hand.end() &&
      validTrainIds.count(tilePlay.m_placeId) > 0;
    if (validPlay) {
      Train& targetTrain = m_board.getTrainById(tilePlay.m_placeId);
      int32 endPips = m_board.m_centerTile->m_highPips;
      if (targetTrain.m_tiles.size() > 0) {
        endPips = targetTrain.m_tiles.back().m_isFlipped ?
          targetTrain.m_tiles.back().m_tile.m_highPips :
          targetTrain.m_tiles.back().m_tile.m_lowPips;
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
        *activeDoubles = false;
        *activeDoublesTrainId = NULL_ID;
        if (player.m_hand.size() == 0) {
          *roundOver = true;
        } else if (playedTile.m_highPips == playedTile.m_lowPips) {
          int32 countPlayed = 0;
          if (m_board.m_centerTile->m_highPips == playedTile.m_highPips) {
            countPlayed += 1;
          }
          for (auto& kv : m_board.m_playerTrains) {
            for (auto& trainTile : kv.second.m_tiles) {
              if () { // CONT HERE
              }
            }
          }
          *activeDoubles = true;
          *activeDoublesTrainId = tilePlay.m_placeId;

          // TODO: Make sure therse is a domino left to play on the doubles.
        }
        break;
      }
    }
    player.m_ai->message(illegalPlayMessage);
  }
}
