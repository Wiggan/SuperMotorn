#include "HexagonAddTool.h"
HexagonAddTool::HexagonAddTool(HexaTerrain* pHexa) : HexagonTool(pHexa)  {
}
void    
HexagonAddTool::leftAction(Vector3 pPickedHexa) {
    HexaTerrainResource* resource = mHexa->getTerrainResource();
    Vector3 offset(0.0f, 0.0f, 2*resource->getRadius() + resource->getDistance());
    if ( pPickedHexa.getX() <= 1 || pPickedHexa.getZ() <= 1 ) {
        resource->enlarge();
        pPickedHexa.setX(pPickedHexa.getX() + 10);
        pPickedHexa.setZ(pPickedHexa.getZ() + 10);
    }
    for ( int i = 0; i < 6; i++ ) {
        offset.rotate(Vector3(0.0f, 1.0f, 0.0f), DirectX::XM_PI / 3);
        Vector3 hexa = resource->getHexaCoordFromLocalCoord(resource->hexa2local(pPickedHexa) + offset);
        if ( !resource->isOccupied(hexa) ) {
            resource->addHexagon(hexa);
        }
    }
}
void    
HexagonAddTool::rightAction(Vector3 pPickedHexa) {
    mHexa->getTerrainResource()->removeHexagon(pPickedHexa);
}
void    
HexagonAddTool::mouseOver(Vector3 pPickedHexa) {
    HexaTerrainResource* resource = mHexa->getTerrainResource();
    Vector3 offset(0.0f, 0.0f, 2*resource->getRadius() + resource->getDistance());
    for ( int i = 0; i < 6; i++ ) {
        offset.rotate(Vector3(0.0f, 1.0f, 0.0f), DirectX::XM_PI / 3);
        Vector3 hexa = resource->getHexaCoordFromLocalCoord(resource->hexa2local(pPickedHexa) + offset);
        if ( !resource->isOccupied(hexa) ) {
            DebugRenderer::instance()->renderMesh(mHexa->getTerrainResource()->getDebugMesh(), mHexa->getTerrainResource()->getTransformFromHexaCoord(hexa)*mHexa->getTerrainResource()->getTransforms()->front());
        }
    }
}
HexagonAddTool::~HexagonAddTool() {
}
