#include "DroneCamera.h"
#include "CameraComponent.h"
#include "DroneEntity.h"
DroneCamera::DroneCamera() {
}
void
DroneCamera::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    CameraComponent* camera = new CameraComponent();
    camera->setPosition(Vector3(0.0f, 10.0f, -30.0f));
    camera->setRotation(Vector3(0.4f, 0.0f, 0.0f));
    add(camera);
}
void    
DroneCamera::update(float pDelta) {
    //float parentYRotation = ((DroneEntity*)mParent)->getRotationMatrix().getRotation().getY();
    float parentYRotation = ((DroneEntity*)mParent)->getWorldRotation().getY();
    float ownYRotation = mRotationMatrix.getRotation().getY();
    float angleY = parentYRotation - ownYRotation;
    mRotationMatrix.rotate(Vector3(0.0f, 1.0f, 0.0f), angleY);
    mDirty = true;
    if ( mDirty ) {
        mUpdatedThisFrame = true;
        mLocalTransform = calculateLocalTransform();
        mWorldTransform = mLocalTransform;
        mDirty = false;
    }
    for ( auto it = mChildren.begin(); it != mChildren.end(); ++it ) {
        (*it)->update(pDelta);
    }
    for ( auto it = mComponents.begin(); it != mComponents.end(); ++it ) {
        (*it)->update(pDelta);
    }
    mUpdatedThisFrame = false;
}
Matrix  
DroneCamera::calculateLocalTransform() {
    return Matrix(mLocalScale, mLocalRotation, mRotationMatrix, mParent->getWorldPosition() + Vector3(0.0f, 0.0f, mParent->getWorldUp().getZ()*5));
}
DroneCamera::~DroneCamera() {
}
