#include "Game.h"

#include "Board.h"
#include "PlayerAI.h"
#include "RNG.h"
#include "Round.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>

Game::Game(std::ostream* out, const GameSettings& gameSettings, std::vector<std::unique_ptr<PlayerAI>>& playerAis)
    : m_out(out), m_gameSettings(gameSettings) {
  if (m_gameSettings.m_numberOfPlayers < 1 || m_gameSettings.m_numberOfPlayers > 8) {
    std::cerr << "There must be 1 - 8 players." << std::endl;
    m_areSettingsValid = false;
  }
  if (m_gameSettings.m_maxPips < 0) {
    std::cerr << "Max pip must be non-negative." << std::endl;
    m_areSettingsValid = false;
  }
  if (m_gameSettings.m_startingHandSize < 0 || m_gameSettings.m_startingHandSize > 25) {
    std::cerr << "Starting hand size must be between 0 and 25." << std::endl;
    m_areSettingsValid = false;
  }

  for (auto& ai : playerAis) {
    m_playerAis.emplace_back(std::move(ai));
  }
}

void Game::run() {
  if (m_out) {
    *m_out << "Number of players: " << m_gameSettings.m_numberOfPlayers << std::endl;
    *m_out << "Max domino pip count: " << m_gameSettings.m_maxPips << std::endl;
    *m_out << "Starting hand size: " << m_gameSettings.m_startingHandSize << std::endl;
  }

  if (!m_areSettingsValid) {
    std::cerr << "Cannot run game with invalid settings." << std::endl;
    return;
  }
  if ((uint32)m_gameSettings.m_numberOfPlayers != m_playerAis.size()) {
    std::cerr << "Number of AIs does not match count in settings." << std::endl;
    return;
  }

  std::vector<Player> players(m_gameSettings.m_numberOfPlayers);
  std::vector<std::vector<EnemyPlayer>> enemyPlayerLists(m_gameSettings.m_numberOfPlayers);
  for (int32 i = 0; i < (int32)players.size(); i++) {
    for (int32 offset = 1; offset < (int32)players.size(); offset++) {
      enemyPlayerLists[i].push_back(players[(i + offset) % players.size()].m_enemyView);
    }
  }
  Board board(players, m_gameSettings);
  std::shuffle(m_playerAis.begin(), m_playerAis.end(), RNG::get().m_mt);
  for (int32 i = 0; i < (int32)players.size(); i++) {
    m_playerAis[i]->setUp(&players[i], &enemyPlayerLists[i], &board);
    players[i].m_ai = m_playerAis[i].get();
  }

  for (auto& ai : m_playerAis) {
    ai->notifyGameStart();
  }

  std::vector<int32> incompleteRounds;
  for (int32 i = 0; i <= m_gameSettings.m_maxPips; i++) {
    incompleteRounds.push_back(i);
  }
  reverse(incompleteRounds.begin(), incompleteRounds.end());
  while (incompleteRounds.size() > 0) {
    Round round(m_gameSettings, board, players, incompleteRounds);
    round.run();
  }

  for (auto& ai : m_playerAis) {
    ai->notifyGameEnd();
  }

  std::vector<int32> playerIndices;
  for (int32 i = 0; i < (int32)players.size(); i++) {
    playerIndices.push_back(i);
  }

  std::sort(playerIndices.begin(), playerIndices.end(), [&](int32 p1, int32 p2) -> bool {
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
    while (endIt != playerIndices.end() && players[*endIt].m_score == players[*startIt].m_score &&
           players[*endIt].m_roundsWon == players[*startIt].m_roundsWon) {
      endIt++;
    }
    std::shuffle(startIt, endIt, RNG::get().m_mt);
    startIt = endIt;
  }
  for (int32 i = 0; i < (int32)players.size(); i++) {
    players[playerIndices[i]].m_ai->notifyGameResult(i + 1);
  }

  m_playerAis.clear(); // in case any AI tries to dereference stuff that's about to go out of scope
}
