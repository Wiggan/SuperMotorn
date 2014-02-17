#pragma once
#include <vector>
#include "BaseCamera.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Entity.h"
#include "Vector4.h"
#include "Timer.h"
class World {
private:
    SeldomConstants             mSeldomConstants;
    std::vector<Entity*>        mEntities;
    Renderer*                   mRenderer;
public:
    void    add(Entity* pEntity);
    void    setDirectionalLight(DirectionalLight* pDirectionalLight);
    void    setSkyBox(Material* pSkyBox);
    void    setFog(float pStart, float pRange, Vector4 pColor, bool pUseSkyColor);
    void    init(ResourceLoader* pResourceLoader, std::vector<BaseCamera*>* pCameras);
    void    tick(float pDelta, const Timer* pTimer);
    void    draw();
            World(Renderer* pRenderer);
            ~World();
};

