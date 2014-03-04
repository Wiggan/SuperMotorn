#include "Entity.h"
#include "BaseCamera.h"
#include "MeshComponent.h"
#include "ColliderComponent.h"
Entity::Entity() {
}
void    
Entity::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    for ( auto it = mChildren.begin(); it != mChildren.end(); ++it ) {
        (*it)->init(pRenderer, pResourceLoader);
    }
    for ( auto it = mComponents.begin(); it != mComponents.end(); ++it ) {
        (*it)->init(pRenderer, pResourceLoader);
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
    mPreviousPosition = mLocalPosition;
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
void            
Entity::setPosition(const Vector3& pPosition) {
    mPreviousPosition = mLocalPosition;
    GameObject::setPosition(pPosition);
}
Vector3         
Entity::getPreviousPosition() {
    return mPreviousPosition;
}
Entity::~Entity() {
}
