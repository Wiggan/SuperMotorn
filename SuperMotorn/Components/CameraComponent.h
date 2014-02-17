#pragma once
#include "Component.h"
#include "BaseCamera.h"
class CameraComponent : public Component, public BaseCamera {
public:
                    CameraComponent();
    virtual Vector3  getPosition();
    virtual Vector3  getLookAt();
    virtual Vector3  getUp();
                    ~CameraComponent();
};

