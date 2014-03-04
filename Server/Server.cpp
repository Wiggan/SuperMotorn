#include "Server.h"
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

Server::Server() : mTeam1Count(0), mTeam2Count(0) {
    WSADATA wsaData;
    std::cout << "Creating server" << std::endl;
    int hr = WSAStartup(MAKEWORD(2,2), &wsaData);
    if ( hr != 0 ) {
        std::cout << "WSAStartup failed: " << hr << std::endl;
    }

    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    hr = getaddrinfo(NULL, "27015", &hints, &result);
    if ( hr != 0 ) {
        std::cout << "getaddrinfo failed: " << hr << std::endl;
        WSACleanup();
        std::cin.get();
        exit(-1);
    }

    mListenSocket = INVALID_SOCKET;
    mListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if ( mListenSocket == INVALID_SOCKET ) {
        std::cout << "socket() failed" << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        std::cin.get();
        exit(-1);
    }

    hr = bind(mListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if ( hr == SOCKET_ERROR ) {
        std::cout << "bind() failed: " << hr << std::endl;
        freeaddrinfo(result);
        closesocket(mListenSocket);
        WSACleanup();
        std::cin.get();
        exit(-1);
    }
    freeaddrinfo(result);
    if ( listen( mListenSocket, SOMAXCONN)  == SOCKET_ERROR ) {
        std::cout << "listen() failed" << std::endl;
        closesocket(mListenSocket);
        WSACleanup();
        std::cin.get();
        exit(-1);
    }
    std::cout << "Server created" << std::endl;

}
void 
Server::waitForClient() {
    std::cout << "Listening..." << std::endl;
    const int maxLength = 512;
    int receiveResult = 0;
    int sendResult = 0;
    char buffer[maxLength];
    fd_set readfs;
    int hr = 0;
    while ( true ) {
        FD_ZERO(&readfs);
        for ( auto it = mClientSockets.begin(); it != mClientSockets.end(); ++it ) {
            FD_SET(it->mSocket, &readfs);
        }
        FD_SET(mListenSocket, &readfs);
        hr = select(NULL, &readfs, NULL, NULL, NULL);
        if ( hr == SOCKET_ERROR ) {
            std::cout << "select() failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            std::cin.get();
            exit(-1);
        }
        if ( FD_ISSET(mListenSocket, &readfs) ) {
            addClient();
        }
        for ( auto it = mClientSockets.begin(); it != mClientSockets.end(); ) {
            if ( FD_ISSET(it->mSocket, &readfs) ) {
                receiveResult = recv(it->mSocket, buffer, maxLength, 0);
                if ( receiveResult > 0 ) {
                    switch ( buffer[0] ) {
                        case FULL_UPDATE: {
                            FullUpdateMessage update(&buffer[0]);
                            std::cout << "Received: " << update.message.toString() << std::endl;
                            break;
                        }
                        default:
                            std::cout << "Received: " << std::string(buffer, receiveResult) << std::endl;
                            break;
                    }
                    for ( auto it2 = mClientSockets.begin(); it2 != mClientSockets.end(); ++it2 ) {
                        if ( it->mSocket != it2->mSocket ) {
                            sendResult = send(it2->mSocket, buffer, receiveResult, 0);
                        }
                    }
                    if ( sendResult == SOCKET_ERROR ) {
                        std::cout << "send() failed: " << WSAGetLastError() << std::endl;
                        closesocket(it->mSocket);
                        WSACleanup();
                        std::cin.get();
                        exit(-1);
                    }
                    ++it;
                } else if( receiveResult == 0 ) {
                    std::cout << "Closing connection" << std::endl;
                    DisconnectedMessage bye;
                    bye.message.command = DISCONNECTED;
                    bye.message.playerId = it->mClientId;
                    for ( auto it2 = mClientSockets.begin(); it2 != mClientSockets.end(); ++it2 ) {
                        if ( it->mSocket != it2->mSocket ) {
                            sendResult = send(it2->mSocket, bye.buffer, sizeof(DisconnectedMessage), 0);
                        }
                    }
                    closesocket(it->mSocket);
                    it = mClientSockets.erase(it);
                } else {
                    std::cout << "recv() failed: " << WSAGetLastError() << std::endl;
                    closesocket(it->mSocket);
                    it = mClientSockets.erase(it);
                }
            } else {
                ++it;
            }
        }
    }
}
void
Server::addClient() {
    Client client;
    client.mSocket = INVALID_SOCKET;
    int sendResult = 0;
    client.mSocket = accept(mListenSocket, NULL, NULL);
    if ( client.mSocket == INVALID_SOCKET ) {
        std::cout << "accept() failed" << std::endl;
        closesocket(mListenSocket);
        WSACleanup();
        std::cin.get();
        exit(-1);
    }
    client.mClientId = (char)mClientSockets.size();
    bool idIsUnique = false;
    while ( !idIsUnique ) {
        idIsUnique = true;
        for ( auto it = mClientSockets.begin(); it != mClientSockets.end(); ++it ) {
            if ( it->mClientId == client.mClientId) {
                idIsUnique = false;
            }
        }
        if ( !idIsUnique ) {
            client.mClientId++;
        }
    }
    if ( mTeam1Count > mTeam2Count ) {
        client.mTeam = 2;
        mTeam2Count++;
    } else {
        client.mTeam = 1;
        mTeam1Count++;
    }
    ConnectedMessage newClient;
    newClient.message.command = CONNECTED;
    newClient.message.playerId = client.mClientId;
    newClient.message.team = client.mTeam;
    ConnectionInfoMessage welcome;
    welcome.message.command = CONNECTION_INFO;
    welcome.message.playerId = client.mClientId;
    welcome.message.team = client.mTeam;
    send(client.mSocket, welcome.buffer, sizeof(ConnectionInfoMessage), 0);
    for ( auto it = mClientSockets.begin(); it != mClientSockets.end(); ++it ) {
        sendResult = send(it->mSocket, newClient.buffer, sizeof(ConnectedMessage), 0);
        ConnectedMessage oldClient;
        oldClient.message.command  = CONNECTED;
        oldClient.message.playerId = it->mClientId;
        oldClient.message.team     = it->mTeam;
        send(client.mSocket, oldClient.buffer, sizeof(ConnectedMessage), 0);
    }
    mClientSockets.push_back(client);
    std::cout << "Accepted connection" << std::endl;
}
Server::~Server() {
}

int main(int argc, char** args) {
    Server server;
    server.waitForClient();
}