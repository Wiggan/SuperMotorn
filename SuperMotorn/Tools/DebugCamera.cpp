#include "DebugCamera.h"
#include "InputComponent.h"
#include "CameraComponent.h"

DebugCamera::DebugCamera() {
}
void    
DebugCamera::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    InputComponent* input = new InputComponent;
    mCamera = new CameraComponent;
    add(input);
    add(mCamera);
    mWindow = pRenderer->getWindow();
    Entity::init(pRenderer, pResourceLoader);
}
void
DebugCamera::update(float pDelta) {
    if ( mW ) {
        setPosition(getLocalPosition() + mCamera->getLocalDirection());
    }
    if ( mS ) {
        setPosition(getLocalPosition() - mCamera->getLocalDirection());
    }
    if ( mA ) {
        setPosition(getLocalPosition() + mCamera->getLocalDirection().cross(Vector3(0.0f, 1.0f, 0.0f)));
    }
    if ( mD ) {
        setPosition(getLocalPosition() - mCamera->getLocalDirection().cross(Vector3(0.0f, 1.0f, 0.0f)));
    }
    Entity::update(pDelta);
}
void    
DebugCamera::keyDown(unsigned int key) {
    switch ( key ) {
    case 'W':
        mW = true;
        break;
    case 'S':
        mS = true;
        break;
    case 'A':
        mA = true;
        break;
    case 'D':
        mD = true;
        break;
    case 'M':
        mMouseLook = !mMouseLook;
    default:
        break;
    }
}
void    
DebugCamera::keyUp(unsigned int key) {
    switch ( key ) {
    case 'W':
        mW = false;
        break;
    case 'S':
        mS = false;
        break;
    case 'A':
        mA = false;
        break;
    case 'D':
        mD = false;
        break;
    default:
        break;
    }
}
void    
DebugCamera::mouseMoved(int x, int y, int winX, int winY) {
    if ( mMouseLook && GetActiveWindow() == mWindow) {
        int centerX = mCamera->getWidth() / 2 + winX;
        int centerY = mCamera->getHeight() / 2 + winY;
        float diffX = (centerX - x) / (float)mCamera->getWidth();
        float diffY = (centerY - y) / (float)mCamera->getHeight();
        Vector3 currentRotation = mCamera->getLocalRotation();
        mCamera->setRotation(Vector3(currentRotation.getX() - diffY, currentRotation.getY() - diffX, 0.0f));
        SetCursorPos(centerX, centerY);
    }
}
CameraComponent*    
DebugCamera::getCamera() {
    return mCamera;
}
DebugCamera::~DebugCamera() {
}
