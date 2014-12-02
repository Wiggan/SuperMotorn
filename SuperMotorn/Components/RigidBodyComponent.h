#pragma once
#include "Component.h"
#include "CollisionListener.h"
class ColliderComponent;
class RigidBodyComponent : public Component, public CollisionListener {
    ColliderComponent*  mCollider;
    Vector3             mCenterOfGravity;
    Vector3             mVelocity;
    Vector3             mAngularVelocity;
    Vector3             mTorque;
    float               mMaxAngularVelocity = 20;
    float               mDrag = 0.0f;
    float               mAngularDrag = 2.1f;
    DirectX::XMFLOAT3   mPreviousCorners[DirectX::BoundingOrientedBox::CORNER_COUNT];
public:
    virtual void        onCollision(const ColliderComponent& pOther);
    virtual void        update(float pDelta);
    void                setVelocity(const Vector3& pVelocity);
    void                setDrag(float pDrag);
    void                setAngularVelocity(const Vector3& pAngularVelocity);
    void                setMaxAngularVelocity(float pMaxAngularVelocity);
    void                setAngularDrag(float pDrag);
    void                addForceAtPoint(Vector3 pPoint, Vector3 pForce, float pTime);
    Vector3             getVelocity();
    Vector3             getAngularVelocity();
                        RigidBodyComponent(ColliderComponent* pCollider);
                        ~RigidBodyComponent();
};

