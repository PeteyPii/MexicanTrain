#include "RemoteTrafficPipe.h"

#include "RequestType.pb.h"
#include <iostream>

RemoteTrafficPipe::RemoteTrafficPipe(GameLobbyManager& gameLobbyManager, ActiveGameManager& activeGameManager)
    : m_gameLobbyManager(gameLobbyManager), m_activeGameManager(activeGameManager) {
}

void RemoteTrafficPipe::replaceSocket(boost::asio::ip::tcp::socket* socket) {
  m_socket.reset(socket);
  if (m_socket) {
    std::cout << "Waiting for data..." << std::endl;
    boost::asio::async_read(
        *m_socket,
        boost::asio::buffer(m_headerBuffer),
        boost::bind(
            &RemoteTrafficPipe::handleHeader,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
  }
}

void RemoteTrafficPipe::handleHeader(const boost::system::error_code& error, size_t bytesRead) {
  if (!error) {
    uint32* requestTypePtr = (uint32*)(m_headerBuffer.data() + 0);
    uint32* dataLengthPtr = (uint32*)(m_headerBuffer.data() + 4);
    m_currentRequest.m_type = (RequestType)boost::asio::detail::socket_ops::network_to_host_long(*requestTypePtr);
    m_currentRequest.m_length = boost::asio::detail::socket_ops::network_to_host_long(*dataLengthPtr);
    m_currentRequest.m_data.reset(new unsigned char[m_currentRequest.m_length]);

    boost::asio::async_read(
        *m_socket,
        boost::asio::buffer(m_currentRequest.m_data.get(), m_currentRequest.m_length),
        boost::bind(
            &RemoteTrafficPipe::handleData,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
  }
}

void RemoteTrafficPipe::handleData(const boost::system::error_code& error, size_t bytesRead) {
  if (!error) {
    std::cout << "Received request: " << m_currentRequest << std::endl;

    boost::asio::async_read(
        *m_socket,
        boost::asio::buffer(m_headerBuffer),
        boost::bind(
            &RemoteTrafficPipe::handleHeader,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
  }
}
