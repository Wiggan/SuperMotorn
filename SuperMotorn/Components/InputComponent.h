#pragma once
#include "Component.h"
class Client;
class InputComponent : public Component {
    Client* mClient;
public:
    InputComponent(Client* pClient);
    void    keyDown(unsigned int key);
    void    keyUp(unsigned int key);
    void    lerpTo(Vector3 pPosition, Vector3 pRotation);
    virtual void update(float pDelta) {}
    ~InputComponent();
};

