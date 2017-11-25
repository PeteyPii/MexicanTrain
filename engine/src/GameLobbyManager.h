#pragma once

class RemoteTrafficPipe;

class GameLobbyManager {
public:
  GameLobbyManager(RemoteTrafficPipe& remoteTrafficPipe);

private:
  RemoteTrafficPipe& m_remoteTrafficPipe;
};
