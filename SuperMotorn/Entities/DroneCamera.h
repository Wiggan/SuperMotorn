#pragma once
#include "Entity.h"
class DroneCamera : public Entity {
    Matrix  mRotationMatrix;
public:
    virtual void    init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    virtual Matrix  calculateLocalTransform();
    virtual void    update(float pDelta);
    DroneCamera();
    ~DroneCamera();
};

