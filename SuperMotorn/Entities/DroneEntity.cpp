#include "DroneEntity.h"
#include "MeshComponent.h"
#include "PropellerComponent.h"
#include "PointLightComponent.h"
#include "DroneCamera.h"
#include "DebugRenderer.h"
#include "RocketLauncher.h"
#include "StartPoint.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "DroneCamera.h"
DroneEntity::DroneEntity(int pPlayerId, int pTeam) : mPlayerId(pPlayerId), mTeam(pTeam) {
}
void
DroneEntity::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    setPosition(mStartPoint->getWorldPosition());
    setRotation(mStartPoint->getWorldRotation());
    MeshComponent* drone = new MeshComponent(pRenderer, pResourceLoader->getResource<Mesh>(L"drone4.dae"));
    auto sockets = drone->getMesh()->getSockets();
    for ( auto it = sockets->begin(); it != sockets->end(); ++it ) {
        if ( it->mName.find("Propeller") != std::string::npos ) {
            PropellerComponent* propeller = new PropellerComponent(pRenderer, (Mesh*)pResourceLoader->getResource<Mesh>(L"propeller.dae"));
            propeller->setMaterial(pResourceLoader->getResource<Material>(L"metal.xml"));
            propeller->setPosition(it->mTransform.getPosition());
            propeller->setInput(it->mName.c_str()[it->mName.find_first_of("_") + 1]);
            mPropellers.push_back(propeller);
            add(propeller);
        }
    }
    if ( mTeam == 1 ) {
        drone->setMaterial(pResourceLoader->getResource<Material>(L"drone4red.xml"));
    } else {
        drone->setMaterial(pResourceLoader->getResource<Material>(L"drone4blue.xml"));
    }
    add(drone);
    
    ColliderComponent* collider = new ColliderComponent(Vector3(3.0f, 0.8f, 4.0f), Vector3(0,0,0), mTeam);
    mRigidBody = new RigidBodyComponent(collider);
    mRigidBody->setAngularDrag(0.9f);
    mRigidBody->setMaxAngularVelocity(0.1f);
    add(collider);
    add(mRigidBody);
    DroneCamera* droneCam = new DroneCamera;
    add(droneCam);
    mRocketLauncher = new RocketLauncher(mTeam);
    mRocketLauncher->setPosition(Vector3(0.0f, 3.0f, 0.0f));
    add(mRocketLauncher);
    
    Entity::init(pRenderer, pResourceLoader);
}
void    
DroneEntity::update(float pDelta) {
    //Vector3 sum;
    for ( auto it = mPropellers.begin(); it != mPropellers.end(); ++it ) {
        mRigidBody->addForceAtPoint((*it)->getLocalPosition(), Vector3(0.0f, (*it)->getForce(), 0.0f), pDelta);
        //sum = sum - (*it)->getLocalPosition()*(*it)->getForce();
    }
    //mRigidBody->setAngularVelocity(mAngularVelocity*0.95f + sum * pDelta);
    /*float forceScalar = 0;
    forceScalar = mAngularVelocity.getLengthEst();
    if ( mAngularVelocity.getLengthEst() > 0.0001f ) {
        Vector3 axis = getLocalUp().cross((mAngularVelocity*mLocalRotation).normalized());
        mLocalRotation.rotate(axis, forceScalar*pDelta + forceScalar*pDelta*pDelta*0.5f);
        mDirty = true;
    }
    for ( auto it = mPropellers.begin(); it != mPropellers.end(); ++it ) {
        forceScalar += (*it)->getForce();
    }
    sum = getWorldUp() * forceScalar * 7;
    Vector3 force = sum + Vector3(0.0f, -9.82f, 0.0f);*/
    
    //if ( mRotateToDesired ) {
    //    using namespace DirectX;
    //    mTimeRotated += pDelta;
    //    mLocalRotation = XMMatrixRotationQuaternion(XMQuaternionSlerp(XMQuaternionRotationMatrix(mLocalRotation), XMQuaternionRotationMatrix(mDesiredRotation), mTimeRotated/4.0f));
    //    if ( mTimeRotated >= 4.0f ) {
    //        mRotateToDesired = false;
    //    }
    //}
    Entity::update(pDelta);
}
void    
DroneEntity::onCollision(const ColliderComponent& pOther) {
    pOther.getOwner()->onEvent(DRONE_LANDED, &mTeam);
    Entity::onCollision(pOther);
} 
void
DroneEntity::setStartPoint(StartPoint* pStartPoint) {
    mStartPoint = pStartPoint;
}
void    
DroneEntity::onEvent(Events pType, void* pObject) {
    switch ( pType ) {
    case ROCKET_HIT:
        setPosition(mStartPoint->getWorldPosition());
        mLocalRotation.rotate(Vector3(0.0f, 1.0f, 0.0f), mStartPoint->getWorldRotation().getY());
        break;
    }
}
void
DroneEntity::keyDown(unsigned int key) {
    for ( auto it = mPropellers.begin(); it != mPropellers.end(); ++it ) {
        (*it)->keyDown(key);
    }
    mRocketLauncher->keyDown(key);
    
}
void    
DroneEntity::keyUp(unsigned int key) {
    for ( auto it = mPropellers.begin(); it != mPropellers.end(); ++it ) {
        (*it)->keyUp(key);
    }
}
int             
DroneEntity::getPlayerId() {
    return mPlayerId;
}
DroneEntity::~DroneEntity() {
}
