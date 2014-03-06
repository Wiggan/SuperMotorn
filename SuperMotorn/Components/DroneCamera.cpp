#include "DroneCamera.h"
#include "Entity.h"
void
DroneCamera::update(float pDelta) {
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
       /* Matrix owner(mOwner->getWorldScale(), 
            Vector3(0.0f, 0.0f, 0.0f), 
            mOwner->getWorldPosition());
        owner.rotate(Vector3(0.0f, 1.0f, 0.0f), mOwner->getWorldRotation().getY());
        mWorldTransform = mLocalTransform*owner;  */
    }
    mUpdatedThisFrame = false;
}
DroneCamera::DroneCamera() {
}
Vector3 
DroneCamera::getPosition() {
    Vector3 pos = mOwner->getWorldPosition();
    Vector3 dir = mOwner->getLocalDirection();
    dir.setY(0.0f);
    return Vector3(pos + dir*-4.0f);
}
Vector3 
DroneCamera::getLookAt() {
    return mOwner->getWorldPosition();
}
Vector3 
DroneCamera::getUp() {
    return Vector3(0.0f, 1.0f, 0.0f);
}
DroneCamera::~DroneCamera() {
}
