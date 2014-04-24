#pragma once
#include "HexagonTool.h"
class HexagonTextureTool : public HexagonTool {
    int                     mTextureIndex = 0;
public:
    HexagonTextureTool(HexaTerrain* pHexa, ResourceLoader* pResourceLoader);
    virtual void    leftAction(Vector3 pPickedHexa);
    virtual void    rightAction(Vector3 pPickedHexa);
    virtual void    mouseOver(Vector3 pPickedHexa);
    virtual void    mouseWheel(int delta);
    ~HexagonTextureTool();
};

