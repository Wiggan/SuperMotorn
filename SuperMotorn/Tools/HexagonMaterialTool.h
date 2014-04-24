#pragma once
#include "HexagonTool.h"
class HexagonMaterialTool : public HexagonTool {
    std::vector<Material*>  mMaterials;
    Material*               mCurrentMaterial;
    int                     mMaterialIndex = 0;
public:
    HexagonMaterialTool(HexaTerrain* pHexa, ResourceLoader* pResourceLoader);
    virtual void    leftAction(Vector3 pPickedHexa);
    virtual void    rightAction(Vector3 pPickedHexa);
    virtual void    mouseOver(Vector3 pPickedHexa);
    virtual void    mouseWheel(int delta);
    ~HexagonMaterialTool();
};

