#include "HexagonYTool.h"

HexagonYTool::HexagonYTool(HexaTerrain* pHexa) : HexagonTool(pHexa) {
}
void    
HexagonYTool::leftAction(Vector3 pPickedHexa) {
    pPickedHexa.setY(pPickedHexa.getY()/mHexa->getTerrainResource()->getIncrement() + mIncrement);
    mHexa->getTerrainResource()->setHexagonY(pPickedHexa);
}
void    
HexagonYTool::rightAction(Vector3 pPickedHexa) {
    Vector3 picked(pPickedHexa);
    picked.setY(pPickedHexa.getY()/mHexa->getTerrainResource()->getIncrement() - mIncrement);
    mHexa->getTerrainResource()->setHexagonY(picked);
}
void    
HexagonYTool::mouseOver(Vector3 pPickedHexa) {
    DebugRenderer::instance()->renderMesh(mHexa->getTerrainResource()->getDebugMesh(), mHexa->getTerrainResource()->getTransformFromHexaCoord(pPickedHexa)*mHexa->getTerrainResource()->getTransforms()->front());
}
void    
HexagonYTool::mouseWheel(int delta) {
    mIncrement += delta;
    if ( mIncrement < 1 ) {
        mIncrement = 1;
    } else if ( mIncrement > 20 ) {
        mIncrement = 20;
    }
}
HexagonYTool::~HexagonYTool() {
}
