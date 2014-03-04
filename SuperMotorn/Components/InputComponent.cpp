#include "Client.h"
#include "InputComponent.h"
#include "Entity.h"
#include <iostream>
InputComponent::InputComponent(Client* pClient) : mClient(pClient) {
}
void
InputComponent::keyDown(unsigned int key) {
    mOwner->keyDown(key);
    if ( mClient != NULL ) {
        mClient->keyDown((char)key);//sendMsg(std::string((char*)&key, 1).append('+'));
    }
}
void
InputComponent::keyUp(unsigned int key) {
    mOwner->keyUp(key);
    if ( mClient != NULL ) {
        mClient->keyUp((char)key);//->sendMsg(std::string((char*)&key, 1).append('-'));
    }
}
void
InputComponent::lerpTo(Vector3 pPosition, Vector3 pRotation) {
    mOwner->setPosition(pPosition);
    mOwner->setRotation(pRotation);
}
InputComponent::~InputComponent() {
}
