#include "Client.h"
#include <iostream>
#include "DroneEntity.h"
Client::Client() {
    mVirtualTimer.reset();
}
bool 
Client::connectTo(const std::string& pIp, const std::string& pPort) {
    WSADATA wsaData;
    std::cout << "Creating client" << std::endl;
    int hr = WSAStartup(MAKEWORD(2,2), &wsaData);
    if ( hr != 0 ) {
        std::cout << "WSAStartup failed: " << hr << std::endl;
        return false;
    }

    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hr = getaddrinfo(pIp.c_str(), pPort.c_str(), &hints, &result);
    if ( hr != 0 ) {
        std::cout << "getaddrinfo failed: " << hr << std::endl;
        WSACleanup();
        return false;
    }
    ptr = result;
    mConnectSocket = INVALID_SOCKET;
    mConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if ( mConnectSocket == INVALID_SOCKET ) {
        std::cout << "socket() failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }
    int tries = 0;
    do {
        tries++;
        if ( ptr != NULL ) {
            hr = connect(mConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            ptr = ptr->ai_next;
        }
    }
    while ( hr == SOCKET_ERROR && tries < 10 );
    if ( hr == SOCKET_ERROR ) {
        std::cout << "connect() failed: " << hr << std::endl;
        freeaddrinfo(result);
        mConnectSocket = INVALID_SOCKET;
        closesocket(mConnectSocket);
        WSACleanup();
        return false;
    }
    mConnected = true;
    std::cout << "Client connected" << std::endl;
    return true;
}

void
Client::receive(int pTimeout) {
    if ( mConnected ) {
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = pTimeout * 1000;
        FD_ZERO(&mReadFs);
        FD_SET(mConnectSocket, &mReadFs);
        select(NULL, &mReadFs, NULL, NULL, &timeout);
        if ( FD_ISSET(mConnectSocket, &mReadFs) ) {
            int receiveResult = recv(mConnectSocket, mBuffer, 512, 0);
            if ( receiveResult > 0 ) {
                //std::cout << "Received: " << std::string(mBuffer, receiveResult) << std::endl;
                int start = 0;
                while ( start < receiveResult ) {
                    start = handleCommand(start);
                }
            }
        }
        mVirtualTimer.tick();
        mSendTransformTimer.tick();
        if ( mSendTransformTimer.getTotalTime() > 0.5f ) {
            mSendTransformTimer.reset();
            sendFullUpdate();
        }
    }
}
int
Client::handleCommand(int pStart) {
    switch ( mBuffer[pStart] ) {
        case UPDATE: {
            UpdateMessage update(&mBuffer[pStart]);
            if ( update.message.action == KEYDOWN ) {
                mRemoteParts[update.message.playerId]->input->keyDown(update.message.key);
            } else if ( update.message.action == KEYUP ) {
                mRemoteParts[update.message.playerId]->input->keyUp(update.message.key);
            }
            std::cout << "UPDATE playerId: " << update.message.playerId << ", timestamp: " << update.message.timestamp 
                << ", key " << update.message.key << update.message.action << std::endl;
            return pStart + sizeof(UpdateMessage);
        }
        case FULL_UPDATE: {
            FullUpdateMessage update(&mBuffer[pStart]);
            std::cout << "Received FULL_UPDATE, rotation, position" << Vector3(update.message.position).toString() << 
            ", rotation: " << Vector3(update.message.rotation).toString() << 
            ", rotation matrix: " << std::endl << Matrix(update.message.rotationMatrix).toString() << std::endl;
            RemotePart* part = mRemoteParts[update.message.playerId];
            part->input->lerpTo(update.message.position, update.message.rotation, update.message.rotationMatrix);
            std::cout << "After update, rotation: " << mRemoteParts[update.message.playerId]->input->getOwner()->getWorldRotation().toString() << std::endl;
            return pStart + sizeof(FullUpdateMessage);
        }
        case CONNECTED: {
            ConnectedMessage connected(&mBuffer[pStart]);
            RemotePart* part = new RemotePart;
            part->playerId   = connected.message.playerId;
            part->team       = connected.message.team;
            part->input      = new InputComponent(NULL);
            mRemoteParts.insert(std::pair<int, RemotePart*>(part->playerId, part));
            std::cout << "CONNECTED playerId:" << part->playerId << " team: " << part->team << std::endl;
            mListener->onPlayerConnected(part->playerId, part->team, part->input);
            return pStart + sizeof(ConnectedMessage);
        }
        case DISCONNECTED: {
            DisconnectedMessage disconnected(&mBuffer[pStart]);
            std::cout << "DISCONNECTED playerId:" << disconnected.message.playerId << std::endl;
            mListener->onPlayerDisconnected(disconnected.message.playerId);
            mRemoteParts.erase(disconnected.message.playerId);
            return pStart + sizeof(DisconnectedMessage);
        }
        case CONNECTION_INFO: {
            mVirtualTimer.reset();
            mSendTransformTimer.reset();
            ConnectionInfoMessage info(&mBuffer[pStart]);
            std::cout << "CONNECTION_INFO playerId:" << info.message.playerId << " team: " << info.message.team << std::endl;
            mLocalDrone = (DroneEntity*)mListener->onSelfConnected(info.message.playerId, info.message.team);
            return pStart + sizeof(ConnectionInfoMessage);
        }
        default:
            std::cout << "Rest of command: " << std::string(mBuffer, pStart+1).substr(pStart) << std::endl;
            return pStart+1;
    }
}
void 
Client::sendMsg(const char* pMsg, int pLength) {
    if ( mConnected ) {
        
        std::cout << "Sending: " << pMsg << std::endl;
        int sendResult = 0;
        sendResult = send(mConnectSocket, pMsg, pLength, 0);
        if ( sendResult == SOCKET_ERROR ) {
            std::cout << "send() failed: " << WSAGetLastError() << std::endl;
            closesocket(mConnectSocket);
            WSACleanup();
            exit(-1);
        }
    }
}
void
Client::sendFullUpdate() {
    FullUpdateMessage update;
    update.message.command = FULL_UPDATE;
    update.message.playerId = (unsigned char)mLocalDrone->getPlayerId();
    update.message.timestamp = mVirtualTimer.getTotalTime();
    update.message.position[0] = mLocalDrone->getLocalPosition().getX();
    update.message.position[1] = mLocalDrone->getLocalPosition().getY();
    update.message.position[2] = mLocalDrone->getLocalPosition().getZ();
    update.message.rotation[0] = mLocalDrone->getLocalRotation().getX();
    update.message.rotation[1] = mLocalDrone->getLocalRotation().getY();
    update.message.rotation[2] = mLocalDrone->getLocalRotation().getZ();
    memcpy(&update.message.rotationMatrix, &(DirectX::XMFLOAT4X4) mLocalDrone->getRotationMatrix(), 16*sizeof(float));
    sendMsg(update.buffer, sizeof(FullUpdateMessage));
    std::cout << "Sending FULL_UPDATE, position" << mLocalDrone->getLocalPosition().toString() << 
        ", rotation: " << mLocalDrone->getLocalRotation().toString() << 
        ", rotation matrix: " << std::endl << mLocalDrone->getRotationMatrix().toString() << std::endl;
}
void
Client::keyDown(char key) {
    UpdateMessage update;
    update.message.playerId = (unsigned char)mLocalDrone->getPlayerId();
    update.message.timestamp = mVirtualTimer.getTotalTime();
    update.message.command = UPDATE;
    update.message.action = KEYDOWN;
    update.message.key = key;
    sendMsg(update.buffer, sizeof(UpdateMessage));
}
void
Client::keyUp(char key) {
    UpdateMessage update;
    update.message.playerId = (unsigned char)mLocalDrone->getPlayerId();
    update.message.timestamp = mVirtualTimer.getTotalTime();
    update.message.command = UPDATE;
    update.message.action = KEYUP;
    update.message.key = key;
    sendMsg(update.buffer, sizeof(UpdateMessage));
}
void 
Client::setListener(NetworkListener* pListener) {
    mListener = pListener;
}
Client::~Client() {
    shutdown(mConnectSocket, SD_SEND);
    closesocket(mConnectSocket);
    WSACleanup();
}


