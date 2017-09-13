#pragma once

#include <set>
#include <string>
#include <vector>
#include "Board.h"
#include "Common.h"
#include "GameSettings.h"
#include "Player.h"


class Game {
public:
  Game(GameSettings gameSettings, Board& board, std::vector<Player>& players);

  int32 playCenterTile(std::vector<int32>* incompleteRounds);
  std::set<int32> standardPlayablePips(Player& player);
  std::set<id> standardPlayableTrains(Player& player);
  bool playerHasPlay(Player& player, std::set<int32> playablePips);
  void playTile(Player& player, std::set<id> validTrainIds, const std::string& illegalPlayMessage, bool* activeDoubles, id* activeDoublesTrainId, bool* roundOver);

private:
  GameSettings m_gameSettings;
  Board& m_board;
  std::vector<Player>& m_players;
};
