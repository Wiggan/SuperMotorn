#include "HexagonMaterialTool.h"


HexagonMaterialTool::HexagonMaterialTool(HexaTerrain* pHexa, ResourceLoader* pResourceLoader) : HexagonTool(pHexa) {
    using namespace std;
#ifdef _DEBUG
    wstring dir = Util::findDirectory(L"..", L"hexagonmaterials");
    vector<wstring> materials = Util::listFilesInDirectory(dir);
#else
    vector<wstring> materials = Util::listFilesInDirectory(Util::findDirectory(L".",L"hexagonmaterials"));
#endif
    for ( auto it = materials.begin(); it != materials.end(); ++it ) {
        mMaterials.push_back(pResourceLoader->getResource<Material>(*it));
    }
    mCurrentMaterial = mMaterials.front();
}
void    
HexagonMaterialTool::leftAction(Vector3 pPickedHexa) {
    HexaTerrainResource* resource = mHexa->getTerrainResource();
    resource->setMaterial(pPickedHexa, mCurrentMaterial);
}
void    
HexagonMaterialTool::rightAction(Vector3 pPickedHexa) {
}
void    
HexagonMaterialTool::mouseOver(Vector3 pPickedHexa) {
    Mesh* mesh = mHexa->getTerrainResource()->getMesh(pPickedHexa);
    if ( mesh ) {
        DebugRenderer::instance()->renderMesh(mesh, Matrix(Vector3(1.02f, 1.02f, 1.02f), Vector3(), Vector3())*
            mHexa->getTerrainResource()->getTransformFromHexaCoord(pPickedHexa) * 
            mHexa->getTerrainResource()->getTransforms()->front(), 
            mCurrentMaterial);
    }
}
void    
HexagonMaterialTool::mouseWheel(int delta) {
    mMaterialIndex = (mMaterialIndex + delta) % mMaterials.size();
    mCurrentMaterial = mMaterials[mMaterialIndex];
}

HexagonMaterialTool::~HexagonMaterialTool() {
}
