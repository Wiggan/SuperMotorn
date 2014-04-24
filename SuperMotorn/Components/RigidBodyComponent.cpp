#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "Entity.h"
#include "DebugRenderer.h"
RigidBodyComponent::RigidBodyComponent(ColliderComponent* pCollider) : mCollider(pCollider) {
}
void    
RigidBodyComponent::update(float pDelta) {
    DebugRenderer::instance()->renderArrow(mOwner->getWorldPosition() + Vector3(0.0f, 4.0f, 0.0f), mAngularVelocity, Vector3(1.0f, 1.0f, mAngularVelocity.getLengthEst()*50));
    mAngularVelocity = mAngularVelocity * mAngularDrag;
    mAngularVelocity.setX(max(min(mAngularVelocity.getX(), mMaxAngularVelocity), -mMaxAngularVelocity));
    mAngularVelocity.setY(max(min(mAngularVelocity.getY(), mMaxAngularVelocity), -mMaxAngularVelocity));
    mAngularVelocity.setZ(max(min(mAngularVelocity.getZ(), mMaxAngularVelocity), -mMaxAngularVelocity));
    if ( mAngularVelocity.getLengthEst() > 0.0001f ) {
        Vector3 axis = mOwner->getLocalUp().cross((mAngularVelocity*mOwner->getRotationMatrix()).normalized());
        DebugRenderer::instance()->renderArrow(mOwner->getWorldPosition() + Vector3(0.0f, 4.0f, 0.0f), axis, Vector3(1.0f, 1.0f, mAngularVelocity.getLengthEst()*50));
        mOwner->rotate(axis, mAngularVelocity.getLengthEst());
    }
    //Vector3 newPosition = getLocalPosition() + mVelocity*pDelta;
    //if ( newPosition.getY() < -40.0f ) {
    //    newPosition.setY(-40.0f);
    //    mVelocity = Vector3(0.0f, 0.0f, 0.0f);
    //}
    //setPosition(newPosition);
    mCollider->getBOB()->GetCorners(&mPreviousCorners[0]);
}
void    
RigidBodyComponent::onCollision(const ColliderComponent& pOther) {
    using namespace DirectX;
    XMFLOAT3 corners[BoundingOrientedBox::CORNER_COUNT];
    mCollider->getBOB()->GetCorners(&corners[0]);
    if ( pOther.getAABB() ) {
        Vector3 diff;
        for ( int i = 0; i < BoundingOrientedBox::CORNER_COUNT; i++ ) {
            if ( pOther.getAABB()->Contains(Vector3(corners[i])) == CONTAINS ) {
                Vector3 currectDiff = Vector3(corners[i]) - Vector3(mPreviousCorners[i]);
                if ( abs(currectDiff.getY()) > diff.getY() ) {
                    diff.setY(currectDiff.getY());
                }
            }
        }
        setPosition(Vector3(getLocalPosition().getX(), getLocalPosition().getY()-diff.getY(), getLocalPosition().getZ()));
    } else if ( pOther.getBOB() ) {
    }
    mVelocity = Vector3(mVelocity.getX(), 0.0f, mVelocity.getZ());
} 
void                
RigidBodyComponent::setVelocity(const Vector3& pVelocity) {
    mVelocity = pVelocity;
}
void                
RigidBodyComponent::setAngularVelocity(const Vector3& pAngularVelocity) {
    mAngularVelocity = pAngularVelocity;
}
void                
RigidBodyComponent::setAngularDrag(float pDrag) {
    mAngularDrag = pDrag;
}
void                
RigidBodyComponent::setMaxAngularVelocity(float pMaxAngularVelocity) {
    mMaxAngularVelocity = pMaxAngularVelocity;
}
void
RigidBodyComponent::addForceAtPoint(Vector3 pPoint, Vector3 pForce, float pTime) {
    mVelocity = mVelocity + pForce*pTime;
    if ( pForce.getLengthEst() > 0.0000001f ) {
        //std::cout << mAngularVelocity.toString() << std::endl;
        mAngularVelocity = mAngularVelocity + (pPoint * pForce.getLengthEst())*pTime;
    }
}
Vector3             
RigidBodyComponent::getVelocity() {
    return mVelocity;
}
Vector3             
RigidBodyComponent::getAngularVelocity() {
    return mAngularVelocity;
}
RigidBodyComponent::~RigidBodyComponent() {
}
