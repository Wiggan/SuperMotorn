#include "DroneEntity.h"
#include "MeshComponent.h"
#include "PropellerComponent.h"
#include "PointLightComponent.h"
#include "DroneCamera.h"
#include "DebugRenderer.h"
#include "RocketLauncher.h"
#include "ColliderComponent.h"
DroneEntity::DroneEntity(int pPlayerId, int pTeam) : mPlayerId(pPlayerId), mTeam(pTeam) {
}
void
DroneEntity::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
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
    drone->setMaterial(pResourceLoader->getResource<Material>(L"drone4.xml"));
    add(drone);
    CameraComponent* droneCam = new CameraComponent();
    droneCam->setPosition(Vector3(0.0f, 10.0f, -25.0f));
    droneCam->setRotation(Vector3(0.4f, 0.0f, 0.0f));
    ColliderComponent* collider = new ColliderComponent(Vector3(3.0f, 0.8f, 4.0f), mTeam);
    add(collider);
    add(droneCam);
    mRocketLauncher = new RocketLauncher(mTeam);
    mRocketLauncher->setPosition(Vector3(0.0f, 3.0f, 0.0f));
    add(mRocketLauncher);
    Entity::init(pRenderer, pResourceLoader);
}
void    
DroneEntity::update(float pDelta) {
    Vector3 sum;
    for ( auto it = mPropellers.begin(); it != mPropellers.end(); ++it ) {
        sum = sum - (*it)->getLocalPosition()*(*it)->getForce();
    }
    mAngularVelocity = mAngularVelocity*0.95f + sum * pDelta;
    float forceScalar = 0;
    forceScalar = mAngularVelocity.getLengthEst();
    if ( mAngularVelocity.getLengthEst() > 0.0001f ) {
        Vector3 axis = getLocalUp().cross((mAngularVelocity*mRotationMatrix).normalized());
        mRotationMatrix.rotate(axis, forceScalar*pDelta + forceScalar*pDelta*pDelta*0.5f);
        mDirty = true;
    }
    for ( auto it = mPropellers.begin(); it != mPropellers.end(); ++it ) {
        forceScalar += (*it)->getForce();
    }
    sum = getWorldUp() * forceScalar * 7;
    Vector3 force = sum + Vector3(0.0f, -9.82f, 0.0f);
    Vector3 newPosition = getLocalPosition() + mVelocity*pDelta + force*pDelta*pDelta*0.5f;
    if ( newPosition.getY() < -40.0f ) {
        newPosition.setY(-40.0f);
        mVelocity = Vector3(0.0f, 0.0f, 0.0f);
    }
    setPosition(newPosition);
    mVelocity = mVelocity*0.985f + force * pDelta;
    //std::cout << "Player " << mPlayerId << ", mVelocity: " << mVelocity.getLengthEst() << std::endl;
    Entity::update(pDelta);
}
void    
DroneEntity::onCollision(const ColliderComponent& pOther) {
    mVelocity = Vector3(mVelocity.getX(), 0.0f, mVelocity.getZ());
    setPosition(Vector3(getLocalPosition().getX(), getPreviousPosition().getY(), getLocalPosition().getZ()));
}
Matrix  
DroneEntity::calculateLocalTransform() {
    return Matrix(mLocalScale, mLocalRotation, mRotationMatrix, mLocalPosition);
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
