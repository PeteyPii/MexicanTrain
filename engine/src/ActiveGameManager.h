#pragma once

class RemoteTrafficPipe;

class ActiveGameManager {
public:
  ActiveGameManager(RemoteTrafficPipe& remoteTrafficPipe);

private:
  RemoteTrafficPipe& m_remoteTrafficPipe;
};
