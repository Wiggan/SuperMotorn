#pragma once
#include "Entity.h"

class PropellerComponent;
class BaseCamera;
class RocketLauncher;
class StartPoint;
class DroneEntity : public Entity {
    int                                 mPlayerId;
    int                                 mTeam;
    std::vector<PropellerComponent*>    mPropellers;
    Vector3                             mCenterOfGravity;
    Vector3                             mVelocity;
    Vector3                             mAngularVelocity;
    Matrix                              mRotationMatrix;
    RocketLauncher*                     mRocketLauncher;
    StartPoint*                         mStartPoint;
public:
    DroneEntity(int pPlayerId, int pTeam);
    virtual void    init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    void            setStartPoint(StartPoint* pStartPoint);
    virtual void    update(float pDelta);
    virtual void    onCollision(const ColliderComponent& pOther);
    virtual void    keyDown(unsigned int key);
    virtual void    keyUp(unsigned int key);
    virtual void    onEvent(Events pType, void* pObject);
    virtual Matrix  calculateLocalTransform();
    int             getPlayerId();
    ~DroneEntity();
};

