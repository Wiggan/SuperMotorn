#include "HexaTerrain.h"
#include "HexaTerrainResource.h"

HexaTerrain::HexaTerrain(const std::wstring& pTerrainName) : mTerrainName(pTerrainName) {
}
void    
HexaTerrain::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    mTerrainResource = pResourceLoader->getResource<HexaTerrainResource>(mTerrainName);
    mRenderer = pRenderer;
    Entity::init(pRenderer, pResourceLoader);
}
void    
HexaTerrain::draw() {
    std::vector<Mesh*>* meshes = mTerrainResource->getMeshes();
    std::vector<Material*>* materials = mTerrainResource->getMaterials();
    std::vector<Matrix>* transforms = mTerrainResource->getTransforms();
    for ( int i = 0; i < meshes->size(); i++ ) {
        mRenderer->drawSolid((*meshes)[i], (*transforms)[i], (*materials)[i]);
    }
}
HexaTerrainResource*    
HexaTerrain::getTerrainResource() {
    return mTerrainResource;
}
HexaTerrain::~HexaTerrain() {
}