#include "HexagonMeshTool.h"
#include "Util.h"
#include <time.h>
HexagonMeshTool::HexagonMeshTool(HexaTerrain* pHexa, ResourceLoader* pResourceLoader) : HexagonTool(pHexa) {
    using namespace std;
#ifdef _DEBUG
    wstring dir = Util::findDirectory(L"..", L"hexagons");
    vector<wstring> meshes = Util::listFilesInDirectory(dir);
#else
    vector<wstring> meshes = Util::listFilesInDirectory(Util::findDirectory(L".",L"hexagons"));
#endif
    for ( auto it = meshes.begin(); it != meshes.end(); ++it ) {
        mMeshes.push_back(pResourceLoader->getResource<Mesh>(*it));
    }
    mCurrentMesh = mMeshes.front();
    srand (time(NULL));
}
void    
HexagonMeshTool::leftAction(Vector3 pPickedHexa) {
    HexaTerrainResource* resource = mHexa->getTerrainResource();
    Vector3 rotation(0.0f, (rand() % 5) * DirectX::XM_PI / 3, 0.0f);
    float scale = 0.75f + (rand() % 5) / 20.0f ;
    resource->addMesh(pPickedHexa, mCurrentMesh, Vector3(scale, scale, scale), rotation);
}
void    
HexagonMeshTool::rightAction(Vector3 pPickedHexa) {
    mHexa->getTerrainResource()->removeMesh(pPickedHexa);
}
void    
HexagonMeshTool::mouseOver(Vector3 pPickedHexa) {
    DebugRenderer::instance()->renderMesh(mCurrentMesh, mHexa->getTerrainResource()->getTransformFromHexaCoord(pPickedHexa)*mHexa->getTerrainResource()->getTransforms()->front());
}
void    
HexagonMeshTool::mouseWheel(int delta) {
    mMeshIndex = (mMeshIndex + delta) % mMeshes.size();
    mCurrentMesh = mMeshes[mMeshIndex];
}
HexagonMeshTool::~HexagonMeshTool() {
}
