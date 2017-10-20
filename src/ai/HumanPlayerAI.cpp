#include "HumanPlayerAI.h"

#include <iostream>
// #include <numeric_limits>
#include "Board.h"
#include "RNG.h"
#include "StatTracker.h"

HumanPlayerAI::HumanPlayerAI(
    const Player& player,
    const std::vector<EnemyPlayer>& enemyPlayers,
    const Board& board,
    std::istream& in,
    std::ostream* out)
    : RandomPlayerAI(player, enemyPlayers, board, out), m_in(in) {
}

HumanPlayerAI::~HumanPlayerAI() {
}

TilePlay HumanPlayerAI::playTile() {
  if (m_makeRandomPlay) {
    return RandomPlayerAI::playTile();
  } else {
    if (m_successfulPlay && m_out) {
      *m_out << "\n";
      *m_out << m_board;
      for (auto& enemyPlayer : m_enemyPlayers) {
        *m_out << enemyPlayer;
      }
      *m_out << m_player;
      *m_out << "\n";
    }

    id tileId;
    id placeId;
    // TODO: Play randomly if stream has ended.
    while (true) {
      if (m_out) {
        *m_out << "ID of tile to play: ";
      }
      m_in >> tileId;
      if (m_in) {
        break;
      } else {
        m_in.clear();
        m_in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
    while (true) {
      if (m_out) {
        *m_out << "ID of place to play tile on: ";
      }
      m_in >> placeId;
      if (m_in) {
        break;
      } else {
        m_in.clear();
        m_in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }

    if (tileId == NULL_ID && placeId == NULL_ID) {
      m_makeRandomPlay = true;
      return RandomPlayerAI::playTile();
    }

    m_successfulPlay = false;
    return TilePlay(tileId, placeId);
  }
}

void HumanPlayerAI::notifyTilePlay(id playerId, id placeId, id tileId) {
  RandomPlayerAI::notifyTilePlay(playerId, placeId, tileId);
  m_makeRandomPlay = false;
  m_successfulPlay = true;
}

void HumanPlayerAI::notifyGameResult(int32 placeFinished) {
  LoggingPlayerAI::notifyGameResult(placeFinished);
  int32 playerCount = 1 + m_enemyPlayers.size();
  Stats& stats = StatTracker::get().m_aiStats["HumanPlayerAI"][playerCount];
  stats.m_finishPlaceCounts[placeFinished] += 1;
  stats.m_scores.push_back(m_player.m_score);
}

void HumanPlayerAI::message(const std::string& msg) {
  if (!m_makeRandomPlay) {
    std::cout << msg << std::endl;
  }
}
