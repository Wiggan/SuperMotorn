#include "BaseCamera.h"
#include <DirectXMath.h>

std::vector<BaseCamera*>    BaseCamera::gCameras;
BaseCamera*                 BaseCamera::gCurrentCamera;

BaseCamera::BaseCamera() {
    gCameras.push_back(this);
}
Vector3 
BaseCamera::getPosition() {
    return Vector3(2.0f, 4.0f, -10.0f);
}
Vector3 
BaseCamera::getUp() {
    return Vector3(0.0f, 1.0f, 0.0f);
}
Vector3 
BaseCamera::getLookAt() {
    return Vector3(0.0f, 0.0f, 0.0f);
}
Matrix 
BaseCamera::getViewTransform() {
    mViewTransform = DirectX::XMMatrixLookAtLH(getPosition(), getLookAt(), getUp());
    return mViewTransform;
}
BaseCamera::~BaseCamera() {
    auto me = std::find(gCameras.begin(), gCameras.end(), this);
    gCameras.erase(me);
}
