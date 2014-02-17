#include "Entity.h"
#include "BaseCamera.h"

Entity::Entity() {
}
void    
Entity::init(Renderer* pRenderer, ResourceLoader* pResourceLoader, std::vector<BaseCamera*>* pCameras) {
    for ( auto it = mChildren.begin(); it != mChildren.end(); ++it ) {
        (*it)->init(pRenderer, pResourceLoader, pCameras);
    }
    for ( auto it = mComponents.begin(); it != mComponents.end(); ++it ) {
        BaseCamera* cam = dynamic_cast<BaseCamera*>(*it);
        if ( cam != NULL ) {
            pCameras->push_back(cam);
        }
    }
}
void
Entity::update(float pDelta) {
    if ( mDirty ) {
        mUpdatedThisFrame = true;
        mLocalTransform = calculateLocalTransform();
        mDirty = false;
    }
    if ( mParent != NULL && mParent->isUpdatedThisFrame() ) {
        mUpdatedThisFrame = true;
    }
    if ( mUpdatedThisFrame ) {
        if ( mParent != NULL ) {
            mWorldTransform = mLocalTransform*mParent->getWorldTransform();
        } else {
            mWorldTransform = mLocalTransform;
        }
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
Entity::calculateLocalTransform() {
    return Matrix(mLocalScale, mLocalRotation, mLocalPosition);
}
void
Entity::draw() {
    for ( auto it = mChildren.begin(); it != mChildren.end(); ++it ) {
        (*it)->draw();
    }
    for ( auto it = mComponents.begin(); it != mComponents.end(); ++it ) {
        (*it)->draw();
    }
}
void            
Entity::add(Component* pComponent) {
    mComponents.push_back(pComponent);
    pComponent->setOwner(this);
}
void            
Entity::add(Entity* pEntity) {
    mChildren.push_back(pEntity);
    pEntity->setParent(this);
}
void
Entity::setParent(Entity* pParent) {
    mParent = pParent;
}
Entity::~Entity() {
}
