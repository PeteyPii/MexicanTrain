#include "GameServerImpl.h"

#include <grpc++/security/server_credentials.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <iostream>
#include <memory>
#include <string>

GameServerImpl::GameServerImpl() : m_gameLobbyManager(*this), m_activeGameManager(*this) {
}

grpc::Status GameServerImpl::sendAndReceive(
    grpc::ServerContext* context, grpc::ServerReaderWriter<ClientRequest, ServerRequest>* stream) {
  ServerRequest request;
  while (stream->Read(&request)) {
    std::cout << "Received request: " << request.DebugString();
  }
  return grpc::Status::OK;
}

void GameServerImpl::run() {
  std::string serverAddress = "0.0.0.0:5000";
  grpc::ServerBuilder builder;
  builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
  builder.RegisterService(this);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << serverAddress << std::endl;
  server->Wait();
}
