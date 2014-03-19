#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "Timer.h"
#include "InputComponent.h"
#include <vector>
#include <unordered_map>
#include "DroneEntity.h"
#include "Matrix.h"
#include "NetworkTypes.h"
class NetworkListener {
public:
    virtual Entity* onSelfConnected(int pPlayerId, int pTeam) = 0;
    virtual void onPlayerConnected(int pPlayerId, int pTeam, InputComponent* pInput) = 0;
    virtual void onPlayerDisconnected(int pPlayerId) = 0;
};
struct RemotePart {
    ~RemotePart() {
    }
    int playerId;
    int team;
    InputComponent* input;
};

class Client {
    NetworkListener*                        mListener;
    SOCKET                                  mConnectSocket;
    bool                                    mConnected = false;
    DroneEntity*                            mLocalDrone;
    Timer                                   mVirtualTimer;
    Timer                                   mSendTransformTimer;
    fd_set                                  mReadFs;
    char                                    mBuffer[512];
    std::unordered_map<int, RemotePart*>    mRemoteParts;

    int                         handleCommand(int start);
public:
    Client();
    void setListener(NetworkListener* pListener);
    bool connectTo(const std::string& pIp, const std::string& pPort);
    void sendMsg(const char* pMsg, int pLength);
    void receive(int pTimeout);
    void sendFullUpdate();
    void keyUp(char pKey);
    void keyDown(char pKey);
    ~Client();
};

