#include "Rocket.h"
#include "Entity.h"
#include "DebugRenderer.h"
#include "MeshComponent.h"
#include "ColliderComponent.h"
Rocket::Rocket(int pTeam) : mFree(true), mTeam(pTeam), mFlyTime(0.0f) {
}
void
Rocket::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    MeshComponent* rocketMesh = new MeshComponent(pRenderer, pResourceLoader->getResource<Mesh>(L"rocket.dae"));
    rocketMesh->setMaterial(pResourceLoader->getResource<Material>(L"rocket.xml"));
    rocketMesh->setScale(Vector3(0.6f, 0.6f, 0.6f));
    add(rocketMesh);
    ColliderComponent* collider = new ColliderComponent(Vector3(0.3f, 0.3f, 0.6f), 3);
    add(collider);
    Entity::init(pRenderer, pResourceLoader);
}
void
Rocket::update(float pDelta) {
    if ( !mFree ) {
        mVelocity = mVelocity + mVelocity * 2.0f*pDelta;
        setPosition(getLocalPosition() + mVelocity*pDelta*0.3f);
        mLocalRotation.rotate(getLocalDirection(), pDelta*3);
        if ( mLocalPosition.getLengthEst() > 1000.0f ) {
            reset();
            return;
        }
        mFlyTime += pDelta;
        Entity::update(pDelta);
    }
}
Matrix  
Rocket::calculateLocalTransform() {
    return Matrix(mLocalScale, mLocalRotation, mLocalPosition);
}
void
Rocket::draw() {
    if ( !mFree ) {
        Entity::draw();
    }
}
bool            
Rocket::isFree() {
    return mFree;
}
void    
Rocket::onCollision(const ColliderComponent& pOther) {
    if ( pOther.getLayer() == mTeam && mFlyTime < 2.0f ) {
        return;
    }
    pOther.getOwner()->onEvent(ROCKET_HIT, this);
    reset();
}
void 
Rocket::reset() {
    mFree = true;
    setPosition(Vector3(0.0f, 0.0f, 0.0f));
    mVelocity = Vector3(0.0f, 0.0f, 0.0f);
    mFlyTime = 0.0f;
    setRotation(Vector3(0.0f, 0.0f, 0.0f));
    mLocalRotation.reset();
}
void            
Rocket::fire(Vector3 pPosition, Vector3 pDirection) {
    setPosition(pPosition);
    setRotation(pDirection.dir2Rot());
    mVelocity = pDirection*25.0f;
    mFree = false;
}
Rocket::~Rocket() {
}
