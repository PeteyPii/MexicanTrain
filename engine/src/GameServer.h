#pragma once

#include "ActiveGameManager.h"
#include "GameLobbyManager.h"
#include "RemoteTrafficPipe.h"
#include <boost/asio.hpp>
#include <memory>

class GameServer {
public:
  GameServer(boost::asio::io_service& ioService);

private:
  void startAccept();
  void handleAccept(const boost::system::error_code& error, boost::asio::ip::tcp::socket* socket);

  boost::asio::ip::tcp::acceptor m_acceptor;
  std::unique_ptr<boost::asio::ip::tcp::socket> m_unhandledSocket;
  GameLobbyManager m_gameLobbyManager;
  ActiveGameManager m_activeGameManager;
  RemoteTrafficPipe m_remoteTrafficPipe;
};
