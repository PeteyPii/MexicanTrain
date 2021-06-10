#include "GameSettingsWrapper.h"

GameSettingsWrapper::GameSettingsWrapper() {
  setNumberOfPlayers(1);
  setMaxPips(6);
  setStartingHandSize(8);
}

GameSettingsWrapper::GameSettingsWrapper(const GameSettingsWrapper& other) {
  m_numberOfPlayers = other.m_numberOfPlayers;
  m_startingHandSize = other.m_startingHandSize;
  m_maxPips = other.m_maxPips;
}

int32 GameSettingsWrapper::numberOfPlayers() const {
  return m_numberOfPlayers;
}

void GameSettingsWrapper::setNumberOfPlayers(int32 val) {
  m_numberOfPlayers = val;
}

int32 GameSettingsWrapper::maxPips() const {
  return m_maxPips;
}

void GameSettingsWrapper::setMaxPips(int32 val) {
  m_maxPips = val;
}

int32 GameSettingsWrapper::startingHandSize() const {
  return m_startingHandSize;
}

void GameSettingsWrapper::setStartingHandSize(int32 val) {
  m_startingHandSize = val;
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
