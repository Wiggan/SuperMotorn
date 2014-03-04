#pragma once
#include "CameraComponent.h"
class DroneCamera :
    public CameraComponent {
public:
    virtual void update(float pDelta);
    virtual Vector3  getPosition();
    virtual Vector3  getLookAt();
    virtual Vector3  getUp();
    DroneCamera();
    ~DroneCamera();
};

