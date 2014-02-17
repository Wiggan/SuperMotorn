#pragma once
#include "Entity.h"
class PropellerComponent;
class BaseCamera;
class DroneEntity : public Entity {
    std::vector<PropellerComponent*>    mPropellers;
    Vector3                             mCenterOfGravity;
    Vector3                             mVelocity;
    Vector3                             mAngularVelocity;
    Matrix                              mRotationMatrix;
public:
    DroneEntity();
    virtual void init(Renderer* pRenderer, ResourceLoader* pResourceLoader, std::vector<BaseCamera*>* pCameras);
    virtual void    update(float pDelta);
    virtual Matrix  calculateLocalTransform();
    void            keyDown(unsigned int key);
    void            keyUp(unsigned int key);
    ~DroneEntity();
};

