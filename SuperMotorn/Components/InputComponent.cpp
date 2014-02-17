#include "InputComponent.h"
#include "Entity.h"

InputComponent::InputComponent() {
}
void    
InputComponent::keyDown(unsigned int key) {
    mOwner->keyDown(key);
}
void    
InputComponent::keyUp(unsigned int key) {
    mOwner->keyUp(key);
}

InputComponent::~InputComponent() {
}
