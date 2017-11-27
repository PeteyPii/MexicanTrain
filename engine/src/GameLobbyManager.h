#pragma once

class GameServerImpl;

class GameLobbyManager {
public:
  GameLobbyManager(GameServerImpl& gameServer);

private:
  GameServerImpl& m_gameServer;
};
