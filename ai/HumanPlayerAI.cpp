#include "HumanPlayerAI.h"

#include "Board.h"
#include "RNG.h"
#include "StatTracker.h"
#include <iostream>

HumanPlayerAI::HumanPlayerAI(std::istream& in, std::ostream* out) : RandomPlayerAI(out), m_in(in) {
}

HumanPlayerAI::~HumanPlayerAI() {
}

TilePlay HumanPlayerAI::playTile() {
  if (m_makeRandomPlay || m_in.eof()) {
    return RandomPlayerAI::playTile();
  } else {
    if (m_successfulPlay && m_out) {
      *m_out << "\n";
      *m_out << *m_board;
      for (auto& enemyPlayer : *m_enemyPlayers) {
        *m_out << enemyPlayer;
      }
      *m_out << *m_player;
      *m_out << "\n";
    }

    id tileId;
    id placeId;
    while (true) {
      if (m_out) {
        *m_out << "ID of tile to play: ";
      }
      m_in >> tileId;
      if (m_in) {
        break;
      } else if (m_in.eof()) {
        return RandomPlayerAI::playTile();
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
      } else if (m_in.eof()) {
        return RandomPlayerAI::playTile();
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
  trackGameResult("HumanPlayerAI", placeFinished);
}

void HumanPlayerAI::message(const std::string& msg) {
  if (!m_makeRandomPlay && !m_in.eof()) {
    if (m_out) {
      *m_out << msg << std::endl;
    }
  }
}
