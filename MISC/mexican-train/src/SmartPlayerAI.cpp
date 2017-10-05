#include "SmartPlayerAI.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <set>
#include <utility>
#include "Board.h"
#include "RNG.h"
#include "StatTracker.h"
#include "Train.h"

using namespace std;


SmartPlayerAI::SmartPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings, std::ostream* out)
  : RandomPlayerAI(player, enemyPlayers, board, gameSettings, out), m_playerTrain(m_board.m_playerTrains.find(m_player.m_id)->second) {
}

SmartPlayerAI::~SmartPlayerAI() {
}

#include <iostream>

TilePlay SmartPlayerAI::playTile() {
  if (!m_board.m_centerTile) {
    m_reevaluatePlays = true;
    return RandomPlayerAI::playTile();
  }

  if (!m_successfulPlay) {
    m_reevaluatePlays = true;
    return RandomPlayerAI::playTile();
  }

  if (m_reevaluatePlays) {
    // TODO: support repeated dominos
    // TODO: always play double domino if possible
    vector<set<int32>> edgeSets(m_gameSettings.m_maxPips + 1);
    for (auto& tile : m_player.m_hand) {
      edgeSets[tile.m_highPips].insert(tile.m_lowPips);
      edgeSets[tile.m_lowPips].insert(tile.m_highPips);
    }

    function<void(int32, int32*, vector<pair<int32, int32>>*)> findBest;
    findBest = [&] (int32 startingPips, int32* retPoints, vector<pair<int32, int32>>* retTrain) {
      set<int32> plays = edgeSets[startingPips];
      int32 bestPoints = 0;
      vector<pair<int32, int32>> bestTrain;
      for (int32 endingPips : plays) {
        edgeSets[startingPips].erase(endingPips);
        edgeSets[endingPips].erase(startingPips);
        int32 points;
        vector<pair<int32, int32>> train;
        findBest(endingPips, &points, &train);
        if (points + startingPips + endingPips >= bestPoints) {
          bestPoints = points + startingPips + endingPips;
          bestTrain = move(train);
          bestTrain.emplace_back(endingPips, startingPips);
        }
        edgeSets[startingPips].insert(endingPips);
        edgeSets[endingPips].insert(startingPips);
      }
      *retPoints = bestPoints;
      *retTrain = move(bestTrain);
    };

    auto findBestRandom = [&] (int32 startingPips, int32* retPoints, vector<pair<int32, int32>>* retTrain) {
      int32 bestPoints = 0;
      vector<pair<int32, int32>> bestTrain;
      for (int32 trial = 0; trial < 1000; trial++) {
        int32 currPips = startingPips;

        int32 points = 0;
        vector<pair<int32, int32>> train;
        while (edgeSets[currPips].size() > 0) {
          int32 randomIndex = RNG::get().m_mt() % edgeSets[currPips].size();
          auto it = edgeSets[currPips].begin();
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

    int32 endPips = m_board.m_centerTile->m_highPips;
    if (m_playerTrain.m_tiles.size() > 0) {
      const TrainTile& endTile = m_playerTrain.m_tiles.back();
      endPips = endTile.m_isFlipped ? endTile.m_tile.m_highPips : endTile.m_tile.m_lowPips;
    }

    int32 bestPoints;
    vector<pair<int32, int32>> bestTrain;
    if (m_player.m_hand.size() < 20) {
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
      for (auto& realTile : m_player.m_hand) {
        if (realTile.m_highPips == high && realTile.m_lowPips == low) {
          m_plannedTrain.push_back(realTile.m_id);
          break;
        }
      }
    }
    assert(m_plannedTrain.size() == bestTrain.size());

    m_reevaluatePlays = false;
  }

  m_successfulPlay = false;
  if (m_plannedTrain.size() > 0) {
    TilePlay tilePlay(m_plannedTrain.back(), m_playerTrain.m_id);
    m_plannedTrain.pop_back();
    return tilePlay;
  }

  m_reevaluatePlays = true;
  return RandomPlayerAI::playTile();
}

void SmartPlayerAI::notifyTilePlay(id playerId, id trainId, id tileId) {
  RandomPlayerAI::notifyTilePlay(playerId, trainId, tileId);
  m_successfulPlay = true;
}

void SmartPlayerAI::notifyGameResult(int32 placeFinished) {
  LoggingPlayerAI::notifyGameResult(placeFinished);
  int32 playerCount = 1 + m_enemyPlayers.size();
  Stats& stats = StatTracker::get().m_aiStats["SmartPlayerAI"][playerCount];
  stats.m_finishPlaceCounts[placeFinished] += 1;
  stats.m_scores.push_back(m_player.m_score);
}

void SmartPlayerAI::message(const std::string& msg) {
}

