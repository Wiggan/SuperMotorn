#include "CameraComponent.h"
#include "Entity.h"

CameraComponent::CameraComponent() {
}
Vector3 
CameraComponent::getPosition() {
    return Component::getWorldPosition();
}
Vector3 
CameraComponent::getLookAt() {
    //return mOwner->getWorldPosition();
    return getWorldDirection() + mOwner->getWorldPosition();
}
Vector3 
CameraComponent::getUp() {
    return Component::getWorldUp();
}
CameraComponent::~CameraComponent() {
}
