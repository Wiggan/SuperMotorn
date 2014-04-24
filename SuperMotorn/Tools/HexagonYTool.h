#pragma once
#include "HexagonTool.h"
class HexagonYTool : public HexagonTool {
    int mIncrement = 1;
public:
    HexagonYTool(HexaTerrain* pHexa);
    virtual void    leftAction(Vector3 pPickedHexa);
    virtual void    rightAction(Vector3 pPickedHexa);
    virtual void    mouseOver(Vector3 pPickedHexa);
    virtual void    mouseWheel(int delta);
    ~HexagonYTool();
};

