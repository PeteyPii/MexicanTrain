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
    vector<set<int32>> edgeLists(m_gameSettings.m_maxPips + 1);
    for (auto& tile : m_player.m_hand) {
      edgeLists[tile.m_highPips].insert(tile.m_lowPips);
      edgeLists[tile.m_lowPips].insert(tile.m_highPips);
    }


    function<void(int32, int32*, vector<pair<int32, int32>>*)> findBest;
    findBest = [&] (int32 seed, int32* retPoints, vector<pair<int32, int32>>* retTrain) {
      set<int32> starts = edgeLists[seed];
      int32 bestPoints = 0;
      vector<pair<int32, int32>> bestTrain;
      for (int32 start : starts) {
        edgeLists[seed].erase(start);
        edgeLists[start].erase(seed);
        int32 points;
        vector<pair<int32, int32>> train;
        findBest(start, &points, &train);
        if (points + seed + start >= bestPoints) {
          bestPoints = points + seed + start;
          bestTrain = move(train);
          bestTrain.emplace_back(start, seed);
        }
        edgeLists[seed].insert(start);
        edgeLists[start].insert(seed);
      }
      *retPoints = bestPoints;
      *retTrain = move(bestTrain);
    };

    int32 seed = m_board.m_centerTile->m_highPips;
    if (m_playerTrain.m_tiles.size() > 0) {
      const TrainTile& endTile = m_playerTrain.m_tiles.back();
      seed = endTile.m_isFlipped ? endTile.m_tile.m_highPips : endTile.m_tile.m_lowPips;
    }

    int32 bestPoints;
    vector<pair<int32, int32>> bestTrain;
    findBest(seed, &bestPoints, &bestTrain);

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

  if (m_plannedTrain.size() > 0) {
    m_successfulPlay = false;
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

