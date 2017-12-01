#pragma once

#include "Common.h"
#include "GameSettings.pb.h"

class GameSettingsWrapper {
public:
  GameSettingsWrapper();
  GameSettingsWrapper(const GameSettingsWrapper& other);

  int32 numberOfPlayers() const;
  void setNumberOfPlayers(int32 val);
  int32 maxPips() const;
  void setMaxPips(int32 val);
  int32 startingHandSize() const;
  void setStartingHandSize(int32 val);

  bool isValid() const;

private:
  GameSettings m_settings;
};
