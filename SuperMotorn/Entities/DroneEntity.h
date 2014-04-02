#pragma once
#include "Entity.h"
class PropellerComponent;
class RigidBodyComponent;
class BaseCamera;
class RocketLauncher;
class StartPoint;
class DroneEntity : public Entity {
    int                                 mPlayerId;
    int                                 mTeam;
    std::vector<PropellerComponent*>    mPropellers;
    float                               mTimeSinceFullUpdate;
    RocketLauncher*                     mRocketLauncher;
    StartPoint*                         mStartPoint;
    RigidBodyComponent*                 mRigidBody;
public:
    DroneEntity(int pPlayerId, int pTeam);
    virtual void    init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    void            setStartPoint(StartPoint* pStartPoint);
    virtual void    update(float pDelta);
    virtual void    onCollision(const ColliderComponent& pOther);
    virtual void    keyDown(unsigned int key);
    virtual void    keyUp(unsigned int key);
    virtual void    onEvent(Events pType, void* pObject);
    int             getPlayerId();
    ~DroneEntity();
};

