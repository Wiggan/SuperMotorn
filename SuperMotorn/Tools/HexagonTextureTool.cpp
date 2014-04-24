#include "HexagonTextureTool.h"


HexagonTextureTool::HexagonTextureTool(HexaTerrain* pHexa, ResourceLoader* pResourceLoader) : HexagonTool(pHexa) {
}
void    
HexagonTextureTool::leftAction(Vector3 pPickedHexa) {
    HexaTerrainResource* resource = mHexa->getTerrainResource();
    resource->setTexture(pPickedHexa, mTextureIndex);
}
void    
HexagonTextureTool::rightAction(Vector3 pPickedHexa) {
}
void    
HexagonTextureTool::mouseOver(Vector3 pPickedHexa) {
    DebugRenderer::instance()->renderMesh(mHexa->getTerrainResource()->getDebugMesh(), mHexa->getTerrainResource()->getTransformFromHexaCoord(pPickedHexa)*mHexa->getTerrainResource()->getTransforms()->front());
}
void    
HexagonTextureTool::mouseWheel(int delta) {
    mTextureIndex = (mTextureIndex + delta) % 3;
}

HexagonTextureTool::~HexagonTextureTool() {
}
