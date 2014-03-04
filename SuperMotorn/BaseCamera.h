#pragma once
#include "Matrix.h"
#include "Vector3.h"
#include <vector>
class BaseCamera {
private:
    Matrix mViewTransform;
public:
    static std::vector<BaseCamera*>  mCameras;
                    BaseCamera();
    Matrix          getViewTransform();
    virtual Vector3  getPosition();
    virtual Vector3  getLookAt();
    virtual Vector3  getUp();
    virtual         ~BaseCamera();
};

