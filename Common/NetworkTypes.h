#pragma once
#include <winsock2.h>
#include <sstream>
enum commands {
    UPDATE = 'U',
    FULL_UPDATE = 'F',
    CONNECTED = 'C',
    DISCONNECTED = 'D',
    CONNECTION_INFO = 'I',
    KEYDOWN = '+', 
    KEYUP = '-'
};
struct ConnectionInfo {
    unsigned char command;
    unsigned char playerId;
    unsigned char team;
};
union ConnectionInfoMessage {
    char buffer[sizeof(ConnectionInfo)];
    ConnectionInfo message;
    ConnectionInfoMessage() {}
    ConnectionInfoMessage(char* pBuffer) {
        memcpy(this, pBuffer, sizeof(*this));
    }
};
struct Disconnected {
    unsigned char command;
    unsigned char playerId;
};
union DisconnectedMessage {
    char buffer[sizeof(Disconnected)];
    Disconnected message;
    DisconnectedMessage() {}
    DisconnectedMessage(char* pBuffer) {
        memcpy(this, pBuffer, sizeof(*this));
    }
};
struct Connected {
    unsigned char command;
    unsigned char playerId;
    unsigned char team;
};
union ConnectedMessage {
    char buffer[sizeof(Connected)];
    Connected message;
    ConnectedMessage() {}
    ConnectedMessage(char* pBuffer) {
        memcpy(this, pBuffer, sizeof(*this));
    }
};
struct Update {
    unsigned char command;
    unsigned char playerId;
    float timestamp;
    unsigned char key;
    unsigned char action;
};
union UpdateMessage {
    char buffer[sizeof(Update)];
    Update message;
    UpdateMessage() {}
    UpdateMessage(char* pBuffer) {
        memcpy(this, pBuffer, sizeof(*this));
    }
};
struct FullUpdate {
    unsigned char command;
    unsigned char playerId;
    float timestamp;
    float rotationMatrix[4][4];
    float position[3];
    float rotation[3];
    std::string toString() {
        std::ostringstream buf;
        buf << "Player " << (int)playerId << " is at time " << timestamp << std::endl;
            /*"[" << transform[0][0] << ", " <<  transform[0][1] << ", " <<  transform[0][2] << ", " <<  transform[0][3] << "]" <<
            "[" << transform[1][0] << ", " <<  transform[1][1] << ", " <<  transform[1][2] << ", " <<  transform[1][3] << "]" <<
            "[" << transform[2][0] << ", " <<  transform[2][1] << ", " <<  transform[2][2] << ", " <<  transform[2][3] << "]" <<
            "[" << transform[3][0] << ", " <<  transform[3][1] << ", " <<  transform[3][2] << ", " <<  transform[3][3] << "]" << std::endl;*/
        return buf.str();
    }
};
union FullUpdateMessage {
    char buffer[sizeof(FullUpdate)];
    FullUpdate message;
    FullUpdateMessage() {}
    FullUpdateMessage(char* pBuffer) {
        memcpy(this, pBuffer, sizeof(*this));
    }
};
