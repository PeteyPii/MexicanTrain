#include "LoggingPlayerAI.h"

#include <sstream>
#include <string>
#include "Board.h"
#include "Player.h"


LoggingPlayerAI::LoggingPlayerAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, const GameSettings& gameSettings, std::ostream* out)
  : PlayerAI(player, enemyPlayers, board, gameSettings),
  m_out(out) {
}

LoggingPlayerAI::~LoggingPlayerAI() {
}

void LoggingPlayerAI::message(const std::string& msg) {
  if (m_out) {
    *m_out << messagePrefix() << "Received message: \"" << msg << "\"\n";
  }
}

void LoggingPlayerAI::notifyGameStart() {
  if (m_out) {
    *m_out << messagePrefix() << "Game started.\n";
  }
}

void LoggingPlayerAI::notifyGameEnd() {
  if (m_out) {
    *m_out << messagePrefix() << "Game ended.\n";
  }
}

void LoggingPlayerAI::notifyRoundStart() {
  if (m_out) {
    *m_out << messagePrefix() << "Round started.\n";
  }
}

void LoggingPlayerAI::notifyRoundEnd() {
  if (m_out) {
    *m_out << messagePrefix() << "Round ended.\n";
  }
}

void LoggingPlayerAI::notifyTilePlay(id playerId, id placeId, id tileId) {
  if (m_out) {
    std::string tileStr;
    if (placeId == m_board.m_centerPlaceId) {
      tileStr = toString(*m_board.m_centerTile);
    } else {
      tileStr = toString(m_board.getTrainById(placeId).m_tiles.back());
    }
    *m_out << messagePrefix() << "{" << playerId << "} played {" << tileId << "}=" << tileStr << " onto {" << placeId << "}.\n";
  }
}

void LoggingPlayerAI::notifyTileDraw(id playerId) {
  if (m_out) {
    if (m_player.m_id == playerId) {
      *m_out << messagePrefix() << "{" << playerId << "} drew " << m_player.m_hand.back() << ".\n";
    } else {
      *m_out << messagePrefix() << "{" << playerId << "} drew a tile.\n";
    }
  }
}

void LoggingPlayerAI::notifyPassTurn(id playerId) {
  if (m_out) {
    *m_out << messagePrefix() << "{" << playerId << "} passed their turn.\n";
  }
}

std::string LoggingPlayerAI::messagePrefix() {
  std::stringstream ss;
  ss << "<" << m_player.m_id << "> ";
  return ss.str();
}
