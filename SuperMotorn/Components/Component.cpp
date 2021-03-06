#include "Component.h"
#include "Entity.h"

Component::Component() {
}
void    
Component::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
}
void
Component::update(float pDelta) {
    if ( mDirty ) {
        mUpdatedThisFrame = true;
        mLocalTransform = Matrix(mLocalScale, mLocalRotation, mLocalPosition);
        mDirty = false;
    }
    if ( mOwner->isUpdatedThisFrame() ) {
        mUpdatedThisFrame = true;
    }
    if ( mUpdatedThisFrame ) {
        mWorldTransform = mLocalTransform*mOwner->getWorldTransform();
    }
    mUpdatedThisFrame = false;
}
void            
Component::setPosition(const Vector3& pPosition) {
    mPreviousPosition = mLocalPosition;
    GameObject::setPosition(pPosition);
}
void
Component::draw() {
}
void
Component::setOwner(Entity* pOwner) {
    mOwner = pOwner;
}
Entity*                 
Component::getOwner() const {
    return mOwner;
}
Component::~Component() {
}
