#include "CameraComponent.h"
#include "Entity.h"

CameraComponent::CameraComponent() {
}
void    
CameraComponent::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    mProjection = pRenderer->getProjection();
    mWidth = pRenderer->getWidth();
    mHeight = pRenderer->getHeight();
}
const Matrix&           
CameraComponent::getProjection() {
    return mProjection;
}
int
CameraComponent::getWidth() {
    return mWidth;
}
int
CameraComponent::getHeight() {
    return mHeight;
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
