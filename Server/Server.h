#pragma once
#include <winsock2.h>
#include <vector>
#include "Client.h"
#include "NetworkTypes.h"
class Server {
    std::vector<Client> mClientSockets;
    SOCKET mListenSocket;
    int mTeam1Count;
    int mTeam2Count;
public:
    Server();
    void waitForClient();
    void addClient();
    ~Server();
};

