#pragma once
#include "HexagonTool.h"
class HexagonAddTool : public HexagonTool {
public:
    HexagonAddTool(HexaTerrain* pHexa);
    virtual void    leftAction(Vector3 pPickedHexa);
    virtual void    rightAction(Vector3 pPickedHexa);
    virtual void    mouseOver(Vector3 pPickedHexa);
    ~HexagonAddTool();
};

