#pragma once

#include "Board.h"
#include "Common.h"
#include "GameSettingsWrapper.h"
#include "Player.h"
#include <set>
#include <string>
#include <vector>

class Round {
public:
  Round(
      GameSettingsWrapper& gameSettings,
      Board& board,
      std::vector<Player>& players,
      std::vector<int32>& incompleteRounds);
  void run();

private:
  GameSettingsWrapper& m_gameSettings;
  Board& m_board;
  std::vector<Player>& m_players;
  std::vector<int32>& m_incompleteRounds;
  bool m_isRoundOver;
  bool m_areDoublesActive;
  id m_activeDoublesTrainId;

  int32 playCenterTile();
  std::set<int32> standardPlayablePips(Player& player);
  std::set<id> standardPlayableTrains(Player& player);
  bool playerHasPlay(Player& player, std::set<int32> playablePips);
  void playTile(Player& player, std::set<id> validTrainIds, const std::string& illegalPlayMessage);
};
