#pragma once
#include "Entity.h"
class CameraComponent;
class DebugCamera : public Entity {
    CameraComponent*    mCamera;
    HWND                mWindow;
    bool                mMouseLook = false;
    bool                mW = false;
    bool                mS = false;
    bool                mA = false;
    bool                mD = false;
public:
    DebugCamera();
    virtual void        init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    virtual void        update(float pDelta);
    virtual void        keyDown(unsigned int key);
    virtual void        keyUp(unsigned int key);
    virtual void        mouseMoved( int x, int y, int winX, int winY);
    CameraComponent*    getCamera();
    ~DebugCamera();
};

