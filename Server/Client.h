#pragma once
#include <WinSock2.h>
class Client {
public:
    SOCKET  mSocket;
    char    mClientId;
    char    mTeam;
    Client();
    ~Client();
};

