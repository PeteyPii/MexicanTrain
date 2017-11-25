#include "GameServer.h"

#include <boost/bind.hpp>
#include <iostream>

using boost::asio::ip::tcp;

GameServer::GameServer(boost::asio::io_service& ioService)
    : m_acceptor(ioService, tcp::endpoint(tcp::v4(), 5000)),
      m_gameLobbyManager(m_remoteTrafficPipe),
      m_activeGameManager(m_remoteTrafficPipe),
      m_remoteTrafficPipe(m_gameLobbyManager, m_activeGameManager) {
  std::cout << "Started listening..." << std::endl;
  startAccept();
}

void GameServer::startAccept() {
  tcp::socket* socket = new tcp::socket(m_acceptor.get_io_service());
  m_unhandledSocket.reset(socket);
  m_acceptor.async_accept(
      *socket, boost::bind(&GameServer::handleAccept, this, boost::asio::placeholders::error, socket));
}

void GameServer::handleAccept(const boost::system::error_code& error, tcp::socket* socket) {
  if (!error) {
    std::cout << "Connected!" << std::endl;
    m_unhandledSocket.release();
    m_remoteTrafficPipe.replaceSocket(socket);
  }

  startAccept();
}
