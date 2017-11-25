#include "GameServer.h"
#include <iostream>

int main(int argc, const char** argv) {
  try {
    boost::asio::io_service ioService;
    GameServer gameServer(ioService);
    ioService.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
