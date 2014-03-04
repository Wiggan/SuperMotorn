#include "ColliderComponent.h"
#include "Entity.h"
#include "DebugRenderer.h"

std::vector<ColliderComponent*> ColliderComponent::mColliders;

ColliderComponent::ColliderComponent(Vector3 pExtents) : mAABB(new DirectX::BoundingBox(Vector3(), pExtents)) {
    mColliders.push_back(this);
}
void
ColliderComponent::update(float pDelta) {
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
        if ( mAABB ) {
            mAABB->Center = getWorldPosition();
            for ( auto it = mColliders.begin(); it != mColliders.end(); ++it ) {
                if ( (*it)->getAABB() != mAABB ) {
                    if ( mAABB->Intersects(*(*it)->getAABB()) ) {
                        mOwner->onCollision(**it);
                    }
                }
            }
        }
    }
    DebugRenderer::instance()->renderCube(mAABB->Center, Vector3(0.0f, 0.0f, 0.0f), Vector3(mAABB->Extents));
    mUpdatedThisFrame = false;
}
DirectX::BoundingBox*   
ColliderComponent::getAABB() {
    return mAABB;
}
ColliderComponent::~ColliderComponent() {
}
