#include "LoggingAI.h"

#include <sstream>
#include "Board.h"
#include "Player.h"


LoggingAI::LoggingAI(const Player& player, const std::vector<EnemyPlayer>& enemyPlayers, const Board& board, std::ostream* out)
  : PlayerAI(player, enemyPlayers, board),
  m_out(out) {
}

LoggingAI::~LoggingAI() {
}

void LoggingAI::message(const std::string& msg) {
  if (m_out) {
    *m_out << messagePrefix() << "Received message: \"" << msg << "\"\n";
  }
}

void LoggingAI::notifyGameStart() {
  if (m_out) {
    *m_out << messagePrefix() << "Game started.\n";
  }
}

void LoggingAI::notifyGameEnd() {
  if (m_out) {
    *m_out << messagePrefix() << "Game ended.\n";
  }
}

void LoggingAI::notifyRoundStart() {
  if (m_out) {
    *m_out << messagePrefix() << "Round started.\n";
  }
}

void LoggingAI::notifyRoundEnd() {
  if (m_out) {
    *m_out << messagePrefix() << "Round ended.\n";
  }
}

void LoggingAI::notifyTilePlay(id playerId, id trainId, id tileId) {
  if (m_out) {
    const Tile& tilePlayed = m_board.getTrainById(trainId).m_tiles.back();
    *m_out << messagePrefix() << "{" << playerId << "} played {" <<
      tileId << "}=[" << tilePlayed.m_highPip << ":" << tilePlayed.m_lowPip << "] onto {" << trainId << "}.\n";
  }
}

void LoggingAI::notifyTileDraw(id playerId) {
  if (m_out) {
    *m_out << messagePrefix() << "{" << playerId << "} drew a tile.\n";
  }
}

std::string LoggingAI::messagePrefix() {
  std::stringstream ss;
  ss << "<" << m_player.m_id << "> ";
  return ss.str();
}
