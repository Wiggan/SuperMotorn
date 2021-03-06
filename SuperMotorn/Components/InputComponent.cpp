#include "Client.h"
#include "InputComponent.h"
#include "Entity.h"
#include "DroneEntity.h"
#include <iostream>
std::vector<InputInterface*> InputComponent::inputs;
InputComponent::InputComponent(Client* pClient) : mClient(pClient) {
}
InputComponent::InputComponent() : mClient(NULL) {
}
void    
InputComponent::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    inputs.push_back(this);
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
InputComponent::mouseMoved(int x, int y, int winX, int winY) {
    mOwner->mouseMoved(x, y, winX, winY);
}
void
InputComponent::lerpTo(Vector3 pPosition, Vector3 pRotation, Matrix pRotationMatrix) {
    mOwner->setPosition(pPosition);
    mOwner->setRotation(pRotation);
    ((DroneEntity*)mOwner)->setRotation(pRotationMatrix);
}
InputComponent::~InputComponent() {
}
