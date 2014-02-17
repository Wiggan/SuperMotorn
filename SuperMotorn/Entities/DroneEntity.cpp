#include "DroneEntity.h"
#include "MeshComponent.h"
#include "PropellerComponent.h"
#include "PointLightComponent.h"
#include "CameraComponent.h"
#include "DebugRenderer.h"
DroneEntity::DroneEntity() {
}
void
DroneEntity::init(Renderer* pRenderer, ResourceLoader* pResourceLoader, std::vector<BaseCamera*>* pCameras) {
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
    droneCam->setPosition(Vector3(0.0f, 10.0f, -15.0f));
    droneCam->setRotation(Vector3(0.4f, 0.0f, 0.0f));
    add(droneCam);
    pCameras->push_back(droneCam);
}
void    
DroneEntity::update(float pDelta) {
    Vector3 sum;
    for ( auto it = mPropellers.begin(); it != mPropellers.end(); ++it ) {
        sum = sum - (*it)->getLocalPosition()*(*it)->getForce();
    }

   // Vector3 deltaRotation = mAngularVelocity*pDelta + mAngularVelocity*pDelta*pDelta*0.5f;
    //mAngularVelocity = mAngularVelocity*0.95 + sum * pDelta;

    float forceScalar = 0;
    for ( auto it = mPropellers.begin(); it != mPropellers.end(); ++it ) {
        forceScalar += (*it)->getForce();
    }
    if ( sum.getLengthEst() > 0.0001f ) {
        Vector3 axis = getLocalUp().cross((sum*mRotationMatrix).normalized());
        mRotationMatrix.rotate(axis, forceScalar*pDelta + forceScalar*pDelta*pDelta*0.5f);
        mDirty = true;
        //DebugRenderer::instance()->renderArrow(getWorldPosition()+Vector3(0.0f,0.0f,0.0f), axis.dir2Rot());
        //DebugRenderer::instance()->renderArrow(getWorldPosition()+Vector3(0.0f,0.0f,0.0f), getLocalUp().dir2Rot());
        //DebugRenderer::instance()->renderArrow(getWorldPosition()+Vector3(0.0f,0.0f,0.0f), (sum*mRotationMatrix).dir2Rot());
        //DebugRenderer::instance()->renderCube(getWorldPosition()+Vector3(0.0f,0.0f,0.0f), sum.dir2Rot(), Vector3(0.05f,0.05f,10.0f));
    }










    sum = getWorldUp() * forceScalar * 4;
    Vector3 force = sum + Vector3(0.0f, -9.82f, 0.0f);
    Vector3 newPosition = getLocalPosition() + mVelocity*pDelta + force*pDelta*pDelta*0.5f;
    //newPosition.setY(max(newPosition.getY(), -20.0f));
    newPosition.setY(newPosition.getY());
    //setPosition(newPosition);
    mVelocity = mVelocity + force * pDelta;
    Entity::update(pDelta);
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
}
void    
DroneEntity::keyUp(unsigned int key) {
    for ( auto it = mPropellers.begin(); it != mPropellers.end(); ++it ) {
        (*it)->keyUp(key);
    }
}
DroneEntity::~DroneEntity() {
}
