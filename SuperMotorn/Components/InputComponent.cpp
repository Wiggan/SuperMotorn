#include "Client.h"
#include "InputComponent.h"
#include "Entity.h"
#include "DroneEntity.h"
#include <iostream>
InputComponent::InputComponent(Client* pClient) : mClient(pClient) {
}
void
InputComponent::keyDown(unsigned int key) {
    mOwner->keyDown(key);
    if ( mClient != NULL ) {
        mClient->sendFullUpdate();
        mClient->keyDown((char)key);
    }
}
void
InputComponent::keyUp(unsigned int key) {
    mOwner->keyUp(key);
    if ( mClient != NULL ) {
        mClient->sendFullUpdate();
        mClient->keyUp((char)key);
    }
}
void
InputComponent::lerpTo(Vector3 pPosition, Vector3 pRotation, Matrix pRotationMatrix) {
    mOwner->setPosition(pPosition);
    mOwner->setRotation(pRotation);
    ((DroneEntity*)mOwner)->setRotation(pRotationMatrix);
}
InputComponent::~InputComponent() {
}
