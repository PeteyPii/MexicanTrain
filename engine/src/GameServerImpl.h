#pragma once

#include "ActiveGameManager.h"
#include "GameLobbyManager.h"
#include "services/GameServer.grpc.pb.h"

class GameServerImpl final : public GameServer::Service {
public:
  GameServerImpl();
  grpc::Status sendAndReceive(
      grpc::ServerContext* context, grpc::ServerReaderWriter<ClientRequest, ServerRequest>* stream) override;
  void run();

private:
  GameLobbyManager m_gameLobbyManager;
  ActiveGameManager m_activeGameManager;
};
