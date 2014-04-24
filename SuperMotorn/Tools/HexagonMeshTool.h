#pragma once
#include "HexagonTool.h"
class HexagonMeshTool : public HexagonTool {
    std::vector<Mesh*>  mMeshes;
    Mesh*               mCurrentMesh;
    int                 mMeshIndex = 0;
public:
    HexagonMeshTool(HexaTerrain* pHexa, ResourceLoader* pResourceLoader);
    virtual void    leftAction(Vector3 pPickedHexa);
    virtual void    rightAction(Vector3 pPickedHexa);
    virtual void    mouseOver(Vector3 pPickedHexa);
    virtual void    mouseWheel(int delta);
    ~HexagonMeshTool();
};

