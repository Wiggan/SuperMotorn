#pragma once
#include "Component.h"
#include "BaseCamera.h"
class CameraComponent : public Component, public BaseCamera {
    Matrix              mProjection;
    int                 mWidth;
    int                 mHeight;
public:
                        CameraComponent();
    virtual void        init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    const Matrix&       getProjection();
    int                 getHeight();
    int                 getWidth();
    virtual Vector3     getPosition();
    virtual Vector3     getLookAt();
    virtual Vector3     getUp();
                        ~CameraComponent();
};

