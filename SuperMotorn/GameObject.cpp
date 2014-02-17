#include "GameObject.h"

static int objectCount = 0;
GameObject::GameObject() : mLocalScale(1.0f, 1.0f, 1.0f), mName("Object"+objectCount++) {

}
Matrix          
GameObject::getWorldTransform() {
    return mWorldTransform;
}
Vector3          
GameObject::getLocalPosition() {
    return mLocalPosition;
}
Vector3          
GameObject::getLocalRotation() {
    return mLocalRotation;
}
Vector3          
GameObject::getLocalScale() {
    return mLocalScale;
}
Vector3          
GameObject::getLocalDirection() {
    return mLocalTransform.getDirection();
}
Vector3          
GameObject::getLocalUp() {
    return mLocalTransform.getUp();
}
Vector3          
GameObject::getWorldPosition() {
    return mWorldTransform.getPosition();
}
Vector3          
GameObject::getWorldRotation() {
    return mWorldTransform.getRotation();
}
Vector3          
GameObject::getWorldScale() {
    return mWorldTransform.getScale();
}
Vector3          
GameObject::getWorldDirection() {
    return mWorldTransform.getDirection();
}
Vector3          
GameObject::getWorldUp() {
    return mWorldTransform.getUp();
}
void
GameObject::setScale(const Vector3& pScale) {
    mDirty = true;
    mLocalScale = pScale;
}
void            
GameObject::setPosition(const Vector3& pPosition) {
    mDirty = true;
    mLocalPosition = pPosition; 
}
void           
GameObject::setRotation(const Vector3& pRotation) {
    mDirty = true;
    mLocalRotation = DirectX::XMVectorModAngles(pRotation);
}
bool
GameObject::isUpdatedThisFrame() {
    return mUpdatedThisFrame;
}
GameObject::~GameObject() {
}
