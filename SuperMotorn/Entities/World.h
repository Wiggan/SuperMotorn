#pragma once
#include <vector>
#include "BaseCamera.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Entity.h"
#include "Vector4.h"
#include "Timer.h"
#include "StartPoint.h"
class World {
private:
    SeldomConstants             mSeldomConstants;
    std::vector<Entity*>        mEntities;
    std::vector<StartPoint*>    mStartPointsTeam1;
    std::vector<StartPoint*>    mStartPointsTeam2;
    Renderer*                   mRenderer;
public:
    void    add(Entity* pEntity);
    void    addStartPoint(StartPoint* pEntity);
    StartPoint* getStartPoint(int pTeam, int pPlayerId);
    void    remove(Entity* pEntity);
    void    setDirectionalLight(DirectionalLight* pDirectionalLight);
    void    setFog(float pStart, float pRange, Vector4 pColor, bool pUseSkyColor);
    void    init(ResourceLoader* pResourceLoader);
    void    tick(float pDelta, const Timer* pTimer);
    void    draw();
            World(Renderer* pRenderer);
            ~World();
};

