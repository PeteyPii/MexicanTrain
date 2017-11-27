#include "GameServerImpl.h"
#include <iostream>

int main(int argc, const char** argv) {
  try {
    GameServerImpl gameServer;
    gameServer.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
