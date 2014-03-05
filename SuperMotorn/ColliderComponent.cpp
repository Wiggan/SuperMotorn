#include "ColliderComponent.h"
#include "Entity.h"
#include "DebugRenderer.h"

std::vector<ColliderComponent*> ColliderComponent::mColliders;
bool ColliderComponent::mLayers[4][4] = {
    {true,  true,  true,  true},
    {true,  false, true,  false},
    {true,  true,  false, false},
    {true,  true,  true,  false},
};
ColliderComponent::ColliderComponent(Vector3 pExtents, int pLayer) : mAABB(new DirectX::BoundingBox(Vector3(), pExtents)), mLayer(pLayer) {
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
                if ( mLayers[mLayer][(*it)->getLayer()] && (*it)->getAABB() != mAABB ) {
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
int                     
ColliderComponent::getLayer() const {
    return mLayer;
}
DirectX::BoundingBox*   
ColliderComponent::getAABB() {
    return mAABB;
}
ColliderComponent::~ColliderComponent() {
    auto me = std::find(mColliders.begin(), mColliders.end(), this);
    mColliders.erase(me);
}
