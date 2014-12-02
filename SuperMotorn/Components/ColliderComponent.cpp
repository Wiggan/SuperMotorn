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
ColliderComponent::ColliderComponent(Vector3 pExtents, Vector3 pOrientation, int pLayer) : 
mBOB(new DirectX::BoundingOrientedBox(Vector3(), pExtents, 
Vector4(DirectX::XMQuaternionRotationRollPitchYaw(pOrientation.getX(), pOrientation.getY(), pOrientation.getZ())))), mLayer(pLayer) {
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
                if ( mLayers[mLayer][(*it)->getLayer()] && (*it) != this ) {
                    if ( (*it)->getAABB() != NULL ) {
                        if ( mAABB->Intersects(*(*it)->getAABB()) ) {
                            mOwner->onCollision(**it);
                        }
                    } else if ( (*it)->getBOB() != NULL ) {
                        if ( mAABB->Intersects(*(*it)->getBOB()) ) {
                            mOwner->onCollision(**it);
                        }
                    }
                }
            }
        } else if ( mBOB ) {
            mBOB->Center = getWorldPosition();
            mBOB->Orientation = Vector4(DirectX::XMQuaternionRotationMatrix(mOwner->getRotationMatrix()));
            for ( auto it = mColliders.begin(); it != mColliders.end(); ++it ) {
                if ( mLayers[mLayer][(*it)->getLayer()] && (*it) != this ) {
                    if ( (*it)->getAABB() != NULL ) {
                        if ( mBOB->Intersects(*(*it)->getAABB()) ) {
                            mOwner->onCollision(**it);
                        }
                    } else if ( (*it)->getBOB() != NULL ) {
                        if ( mBOB->Intersects(*(*it)->getBOB()) ) {
                            mOwner->onCollision(**it);
                        }
                    }
                }
            }
        }
    }
    if ( mAABB ) {
        //DebugRenderer::instance()->renderCube(mAABB->Center, Vector3(0.0f, 0.0f, 0.0f), Vector3(mAABB->Extents));
    } else if ( mBOB ) {
        //DebugRenderer::instance()->renderCube(mBOB->Center, mBOB->Orientation, Vector3(mBOB->Extents));
    }
    mUpdatedThisFrame = false;
}
int                     
ColliderComponent::getLayer() const {
    return mLayer;
}
DirectX::BoundingBox*   
ColliderComponent::getAABB() const {
    return mAABB;
}
DirectX::BoundingOrientedBox*   
ColliderComponent::getBOB() const {
    return mBOB;
}
ColliderComponent::~ColliderComponent() {
    auto me = std::find(mColliders.begin(), mColliders.end(), this);
    mColliders.erase(me);
}
