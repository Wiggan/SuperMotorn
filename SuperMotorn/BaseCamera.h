#pragma once
#include "Matrix.h"
#include "Vector3.h"
class BaseCamera {
private:
    Matrix mViewTransform;
public:
                    BaseCamera();
    Matrix          getViewTransform();
    virtual Vector3  getPosition();
    virtual Vector3  getLookAt();
    virtual Vector3  getUp();
    virtual         ~BaseCamera();
};

