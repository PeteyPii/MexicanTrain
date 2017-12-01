#include "GameSettingsWrapper.h"

GameSettingsWrapper::GameSettingsWrapper() {
  setNumberOfPlayers(1);
  setMaxPips(6);
  setStartingHandSize(8);
}

GameSettingsWrapper::GameSettingsWrapper(const GameSettingsWrapper& other) {
  m_settings.CopyFrom(other.m_settings);
}

int32 GameSettingsWrapper::numberOfPlayers() const {
  return m_settings.number_of_players();
}

void GameSettingsWrapper::setNumberOfPlayers(int32 val) {
  m_settings.set_number_of_players(val);
}

int32 GameSettingsWrapper::maxPips() const {
  return m_settings.max_pips();
}

void GameSettingsWrapper::setMaxPips(int32 val) {
  m_settings.set_max_pips(val);
}

int32 GameSettingsWrapper::startingHandSize() const {
  return m_settings.starting_hand_size();
}

void GameSettingsWrapper::setStartingHandSize(int32 val) {
  m_settings.set_starting_hand_size(val);
}

bool GameSettingsWrapper::isValid() const {
  if (numberOfPlayers() < 1 || numberOfPlayers() > 8) {
    return false;
  }
  if (maxPips() < 0 || maxPips() > 18) {
    return false;
  }
  if (startingHandSize() < 0 || startingHandSize() > 30) {
    return false;
  }
  return true;
}
