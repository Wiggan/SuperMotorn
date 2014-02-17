#include "Component.h"
#include "Entity.h"

Component::Component() {
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
Component::draw() {
}
void
Component::setOwner(Entity* pOwner) {
    mOwner = pOwner;
}
Component::~Component() {
}
