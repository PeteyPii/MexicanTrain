#pragma once

#include "Common.h"
#include "Request.h"
#include <array>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>

class GameLobbyManager;
class ActiveGameManager;

class RemoteTrafficPipe {
public:
  RemoteTrafficPipe(GameLobbyManager& gameLobbyManager, ActiveGameManager& activeGameManager);
  void replaceSocket(boost::asio::ip::tcp::socket* socket);

private:
  void handleHeader(const boost::system::error_code& error, size_t bytesRead);
  void handleData(const boost::system::error_code& error, size_t bytesRead);

  GameLobbyManager& m_gameLobbyManager;
  ActiveGameManager& m_activeGameManager;
  std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;
  std::array<unsigned char, Request::Header::SIZE> m_headerBuffer;
  Request m_currentRequest;
};
