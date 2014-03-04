#include "RocketComponent.h"
#include "Entity.h"
#include "DebugRenderer.h"
RocketComponent::RocketComponent(Renderer* pRenderer, Mesh* pMesh) : MeshComponent(pRenderer, pMesh), mFree(true) {
    mLocalScale = Vector3(0.3f, 0.3f, 0.3f);
    //setAABB(Vector3(0.7f, 0.7f, 0.7f));
}
void
RocketComponent::update(float pDelta) {
    if ( !mFree ) {
        mVelocity = mVelocity + mVelocity * 4.0f*pDelta;
        setPosition(getLocalPosition() + mVelocity*pDelta*0.3f);
        mLocalTransform = Matrix(mLocalScale, mLocalRotation, mLocalPosition);
        if ( mLocalPosition.getLengthEst() > 1000.0f ) {
            mFree = true;
        }
       /* if ( mAABB ) {
            mAABB->Center = getLocalPosition();
            DebugRenderer::instance()->renderCube(mAABB->Center, Vector3(0.0f, 0.0f, 0.0f), Vector3(mAABB->Extents));
            for ( auto it = mCollidables->begin(); it != mCollidables->end(); ++it ) {
                if ( (*it)->getAABB() != mAABB ) {
                    if ( mAABB->Intersects(*(*it)->getAABB()) ) {
                        std::cout << "hit" << std::endl;
                    }
                }
            }
        }*/
    }
}
void
RocketComponent::draw() {
    if ( !mFree ) {
        mRenderer->drawSolid(mMesh, mLocalTransform, mMaterial);
    }
}
bool            
RocketComponent::isFree() {
    return mFree;
}
void            
RocketComponent::fire(Vector3 pPosition, Vector3 pDirection) {
    mFree = false;
    setPosition(pPosition);
    setRotation(pDirection.dir2Rot());
    mVelocity = pDirection*8.0f;
}
RocketComponent::~RocketComponent() {
}
