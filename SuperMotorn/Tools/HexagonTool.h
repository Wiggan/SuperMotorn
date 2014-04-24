#pragma once
#include "InputInterface.h"
#include "HexaTerrain.h"
#include "BaseCamera.h"
#include "HexaTerrainResource.h"
#include "DebugRenderer.h"
class HexagonTool : public InputInterface {
    Matrix          mProjection;
    int             mWidth;
    int             mHeight;
    BaseCamera*     mCamera;
    Vector3         mPickedHexa;
protected:
    HexaTerrain*    mHexa;
public:
    HexagonTool(HexaTerrain* pHexa);
    virtual void    keyDown(unsigned int key);
    virtual void    keyUp(unsigned int key);
    virtual void    mouseMoved(int x, int y, int winX, int winY);
    virtual void    mouseWheel(int delta);
    virtual void    mouseDown(unsigned int pButton);
    virtual void    leftAction(Vector3 pPickedHexa) = 0;
    virtual void    rightAction(Vector3 pPickedHexa) = 0;
    virtual void    mouseOver(Vector3 pPickedHexa) = 0;
    void            setCamera(BaseCamera* pCamera);
    void            setProjection(const Matrix& pProjection, int pWidth, int pHeight);
    virtual ~HexagonTool();
};

