#pragma once

class GameServerImpl;

class ActiveGameManager {
public:
  ActiveGameManager(GameServerImpl& gameServer);

private:
  GameServerImpl& m_gameServer;
};
