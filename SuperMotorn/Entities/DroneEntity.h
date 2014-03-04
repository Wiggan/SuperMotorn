#pragma once
#include "Entity.h"

class PropellerComponent;
class BaseCamera;
class RocketLauncherComponent;
class DroneEntity : public Entity {
    int                                 mPlayerId;
    std::vector<PropellerComponent*>    mPropellers;
    Vector3                             mCenterOfGravity;
    Vector3                             mVelocity;
    Vector3                             mAngularVelocity;
    Matrix                              mRotationMatrix;
    RocketLauncherComponent*            mRocketLauncher;
public:
    DroneEntity(int pPlayerId);
    virtual void init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    virtual void    update(float pDelta);
    virtual void    onCollision(const ColliderComponent& pOther);
    virtual Matrix  calculateLocalTransform();
    void            keyDown(unsigned int key);
    void            keyUp(unsigned int key);
    int             getPlayerId();
    ~DroneEntity();
};

