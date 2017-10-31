#include "SmartPlayerAI.h"

#include "Board.h"
#include "RNG.h"
#include "StatTracker.h"
#include "Train.h"
#include <algorithm>
#include <cassert>
#include <functional>
#include <map>
#include <set>
#include <utility>

using namespace std;

SmartPlayerAI::SmartPlayerAI(std::ostream* out) : RandomPlayerAI(out) {
}

SmartPlayerAI::~SmartPlayerAI() {
}

void SmartPlayerAI::setUp(const Player* player, const std::vector<EnemyPlayer>* enemyPlayers, const Board* board) {
  RandomPlayerAI::setUp(player, enemyPlayers, board);
  m_playerTrain = &m_board->m_playerTrains.find(m_player->m_id)->second;
  m_otherTrains.push_back(&m_board->m_publicTrain);
  for (auto& kv : m_board->m_playerTrains) {
    if (kv.first != m_player->m_id) {
      m_otherTrains.push_back(&kv.second);
    }
  }
}

TilePlay SmartPlayerAI::playTile() {
  if (!m_board->m_centerTile) {
    m_reevaluatePlays = true;
    return RandomPlayerAI::playTile();
  }

  if (m_fallbackRandomPlay) {
    return RandomPlayerAI::playTile();
  }

  if (m_reevaluatePlays) {
    // TODO: support repeated dominos
    vector<set<int32>> edgeSets(m_board->m_gameSettings.m_maxPips + 1);
    for (auto& tile : m_player->m_hand) {
      edgeSets[tile.m_highPips].insert(tile.m_lowPips);
      edgeSets[tile.m_lowPips].insert(tile.m_highPips);
    }

    function<void(int32, int32*, vector<pair<int32, int32>>*)> findBest;
    findBest = [&](int32 startingPips, int32* retPoints, vector<pair<int32, int32>>* retTrain) {
      int32 bestPoints = 0;
      vector<pair<int32, int32>> bestTrain;
      auto it = edgeSets[startingPips].find(startingPips);
      int32 points;
      vector<pair<int32, int32>> train;
      if (it != edgeSets[startingPips].end()) {
        edgeSets[startingPips].erase(it);
        findBest(startingPips, &points, &train);
        if (points + 2 * startingPips >= bestPoints) {
          bestPoints = points + 2 * startingPips;
          bestTrain = move(train);
          bestTrain.emplace_back(startingPips, startingPips);
        }
        edgeSets[startingPips].insert(startingPips);
      } else {
        set<int32> plays = edgeSets[startingPips];
        for (int32 endingPips : plays) {
          edgeSets[startingPips].erase(endingPips);
          edgeSets[endingPips].erase(startingPips);
          findBest(endingPips, &points, &train);
          if (points + startingPips + endingPips >= bestPoints) {
            bestPoints = points + startingPips + endingPips;
            bestTrain = move(train);
            bestTrain.emplace_back(endingPips, startingPips);
          }
          edgeSets[startingPips].insert(endingPips);
          edgeSets[endingPips].insert(startingPips);
        }
      }
      *retPoints = bestPoints;
      *retTrain = move(bestTrain);
    };

    auto findBestRandom = [&](int32 startingPips, int32* retPoints, vector<pair<int32, int32>>* retTrain) {
      int32 bestPoints = 0;
      vector<pair<int32, int32>> bestTrain;
      for (int32 trial = 0; trial < 1000; trial++) {
        int32 currPips = startingPips;
        int32 points = 0;
        vector<pair<int32, int32>> train;
        while (edgeSets[currPips].size() > 0) {
          auto it = edgeSets[currPips].find(currPips);
          if (it != edgeSets[currPips].end()) {
            edgeSets[currPips].erase(it);
            points += 2 * currPips;
            train.emplace_back(currPips, currPips);
          } else {
            int32 randomIndex = RNG::get().m_mt() % edgeSets[currPips].size();
            it = edgeSets[currPips].begin();
            for (int32 i = 0; i < randomIndex; i++) {
              it++;
            }
            int32 nextPips = *it;
            edgeSets[currPips].erase(it);
            edgeSets[nextPips].erase(currPips);
            points += currPips + nextPips;
            train.emplace_back(nextPips, currPips);
            currPips = nextPips;
          }
        }

        for (auto& tile : train) {
          edgeSets[tile.first].insert(tile.second);
          edgeSets[tile.second].insert(tile.first);
        }

        if (points >= bestPoints) {
          bestPoints = points;
          bestTrain = move(train);
        }
      }

      *retPoints = bestPoints;
      *retTrain = move(bestTrain);
      reverse(retTrain->begin(), retTrain->end());
    };

    int32 endPips = m_board->m_centerTile->m_highPips;
    if (m_playerTrain->m_tiles.size() > 0) {
      const TrainTile& endTile = m_playerTrain->m_tiles.back();
      endPips = endTile.m_isFlipped ? endTile.m_tile.m_highPips : endTile.m_tile.m_lowPips;
    }

    int32 bestPoints;
    vector<pair<int32, int32>> bestTrain;
    if (m_player->m_hand.size() < 20) {
      findBest(endPips, &bestPoints, &bestTrain);
    } else {
      findBestRandom(endPips, &bestPoints, &bestTrain);
    }

    m_plannedTrain.clear();
    for (auto& tile : bestTrain) {
      int32 high = tile.first;
      int32 low = tile.second;
      if (high < low) {
        swap(high, low);
      }
      for (auto& realTile : m_player->m_hand) {
        if (realTile.m_highPips == high && realTile.m_lowPips == low) {
          m_plannedTrain.push_back(realTile.m_id);
          break;
        }
      }
    }
    assert(m_plannedTrain.size() == bestTrain.size());

    map<id, const Tile*> spareTiles;
    for (auto& tile : m_player->m_hand) {
      spareTiles.emplace(tile.m_id, &tile);
    }
    for (id& tileId : m_plannedTrain) {
      spareTiles.erase(tileId);
    }
    m_spareTiles.clear();
    for (auto& kv : spareTiles) {
      m_spareTiles.push_back(kv.second->m_id);
    }
    sort(m_spareTiles.begin(), m_spareTiles.end(), [&](id left, id right) -> bool {
      const Tile& leftTile = *spareTiles[left];
      const Tile& rightTile = *spareTiles[right];
      return leftTile.m_highPips + leftTile.m_lowPips > rightTile.m_highPips + rightTile.m_lowPips;
    });
    m_spareTrainIndex = 0;
    m_spareTileIndex = 0;

    m_reevaluatePlays = false;
  }

  if (m_playerTrain->m_isPublic && !m_tryingLockPlay) {
    if (m_plannedTrain.size() > 0) {
      m_tryingLockPlay = true;
      return TilePlay(m_plannedTrain.back(), m_playerTrain->m_id);
    }
  }
  while (m_spareTileIndex < m_spareTiles.size()) {
    id tileId = m_spareTiles[m_spareTileIndex];
    auto tileIt = find_if(m_player->m_hand.begin(), m_player->m_hand.end(), [&](const Tile& tile) -> bool {
      return tileId == tile.m_id;
    });
    assert(tileIt != m_player->m_hand.end());
    while (m_spareTrainIndex < m_otherTrains.size()) {
      int32 endPips = m_board->m_centerTile->m_highPips;
      if (m_otherTrains[m_spareTrainIndex]->m_tiles.size() > 0) {
        const TrainTile& endTile = m_otherTrains[m_spareTrainIndex]->m_tiles.back();
        endPips = endTile.m_isFlipped ? endTile.m_tile.m_highPips : endTile.m_tile.m_lowPips;
      }
      m_spareTrainIndex++;
      if (tileIt->m_highPips == endPips || tileIt->m_lowPips == endPips) {
        m_tryingSparePlay = true;
        return TilePlay(m_otherTrains[m_spareTrainIndex - 1]->m_id, tileId);
      }
    }
    m_spareTileIndex++;
    m_spareTrainIndex = 0;
  }
  if (!m_playerTrain->m_isPublic && !m_tryingTrainPlay) {
    if (m_plannedTrain.size() > 0) {
      m_tryingTrainPlay = true;
      return TilePlay(m_plannedTrain.back(), m_playerTrain->m_id);
    }
  }

  m_fallbackRandomPlay = true;
  m_reevaluatePlays = true;
  return RandomPlayerAI::playTile();
}

void SmartPlayerAI::notifyRoundStart() {
  m_reevaluatePlays = true;
}

void SmartPlayerAI::notifyTilePlay(id playerId, id placeId, id tileId) {
  LoggingPlayerAI::notifyTilePlay(playerId, placeId, tileId);
  if (playerId == m_player->m_id && !m_fallbackRandomPlay) {
    if (m_tryingTrainPlay) {
      m_plannedTrain.pop_back();
    } else if (m_tryingSparePlay) {
      auto it = find(m_spareTiles.begin(), m_spareTiles.end(), tileId);
      assert(it != m_spareTiles.end());
      m_spareTiles.erase(it);
    } else if (m_tryingLockPlay) {
      m_plannedTrain.pop_back();
    }
  }
  m_tryingLockPlay = false;
  m_tryingSparePlay = false;
  m_tryingTrainPlay = false;
  m_fallbackRandomPlay = false;
  for (auto train : m_otherTrains) {
    if (train->m_id == placeId) {
      m_spareTrainIndex = 0;
      m_spareTileIndex = 0;
      break;
    }
  }
}

void SmartPlayerAI::notifyTileDraw(id playerId) {
  if (playerId == m_player->m_id) {
    m_reevaluatePlays = true;
  }
}

void SmartPlayerAI::notifyPassTurn(id playerId) {
  if (playerId == m_player->m_id) {
    m_reevaluatePlays = true;
  }
}

void SmartPlayerAI::notifyGameResult(int32 placeFinished) {
  LoggingPlayerAI::notifyGameResult(placeFinished);
  trackGameResult("SmartPlayerAI", placeFinished);
}

void SmartPlayerAI::message(const std::string& msg) {
}
