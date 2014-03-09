#pragma once
#include "Entity.h"
#include "PointLight.h"
class PointLightComponent;
class MeshComponent;
class FlagPlatform : public Entity {
    int                     mCurrentTeam;
    PointLight              mRedLight;
    Material*               mRedMaterial;
    PointLight              mBlueLight;
    Material*               mBlueMaterial;
    PointLightComponent*    mLightComponent;
    MeshComponent*          mFlag;
public:
                    FlagPlatform(int pTeam);
    virtual void    init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    virtual void    onEvent(Events pType, void* pObject);
                    ~FlagPlatform();
};

