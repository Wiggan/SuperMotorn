#include "FlagPlatform.h"
#include "PointLightComponent.h"
#include "MeshComponent.h"
#include "ColliderComponent.h"
FlagPlatform::FlagPlatform(int pTeam) : mCurrentTeam(pTeam) {
}
void
FlagPlatform::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    MeshComponent* platform = new MeshComponent(pRenderer, pResourceLoader->getResource<Mesh>(L"platform2.dae"));
    platform->setMaterial(pResourceLoader->getResource<Material>(L"concrete.xml"));
    mFlag = new MeshComponent(pRenderer, pResourceLoader->getResource<Mesh>(L"flag.dae"));
    mFlag->setPosition(Vector3(-11.35f, 1.0f, 0.95f));
    

    mRedLight.diffuse = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
    mRedLight.specular = Vector4(1.0f, 0.0f, 0.0f, 8.0f);
    mRedLight.attenuation = Vector3(1.8f, 0.25f, 0.0f);
    mRedLight.range = 100.0f;
    mBlueLight.diffuse = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
    mBlueLight.specular = Vector4(0.0f, 0.0f, 1.0f, 8.0f);
    mBlueLight.attenuation = Vector3(1.8f, 0.25f, 0.0f);
    mBlueLight.range = 100.0f;
    mRedMaterial = pResourceLoader->getResource<Material>(L"flagred.xml");
    mBlueMaterial = pResourceLoader->getResource<Material>(L"flagblue.xml");
    if ( mCurrentTeam == 1 ) {
        mLightComponent = new PointLightComponent(pRenderer, mRedLight);
        mFlag->setMaterial(mRedMaterial);
    } else {
        mLightComponent = new PointLightComponent(pRenderer, mBlueLight);
        mFlag->setMaterial(mBlueMaterial);
    }
    mLightComponent->setPosition(Vector3(-11.35f, 3.0f, 0.95f));
    ColliderComponent* collider = new ColliderComponent(Vector3(9.0f, 1.0f, 9.0f));
    add(collider);
    add(platform);
    add(mFlag);
    add(mLightComponent);
    Entity::init(pRenderer, pResourceLoader);
}
void    
FlagPlatform::onEvent(Events pType, void* pObject) {
    switch ( pType ) {
    case DRONE_LANDED:
        int team = *((int*)pObject);
        if ( team != mCurrentTeam ) {
            if ( team == 1 ) {
                mCurrentTeam = 1;
                mLightComponent->setLight(mRedLight);
                mFlag->setMaterial(mRedMaterial);
            } else {
                mCurrentTeam = 2;
                mLightComponent->setLight(mBlueLight);
                mFlag->setMaterial(mBlueMaterial);
            }
        }
        break;
    }
}
FlagPlatform::~FlagPlatform() {
}
