#include "HexaTerrain.h"
#include "HexaTerrainResource.h"

HexaTerrain::HexaTerrain(const std::wstring& pTerrainName) : mTerrainName(pTerrainName) {
}
void    
HexaTerrain::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    mTerrainResource = pResourceLoader->getResource<HexaTerrainResource>(mTerrainName);
    mTerrainMaterial = pResourceLoader->getResource<Material>(Util::string2wstring(mTerrainResource->getMaterial()));
    mRenderer = pRenderer;
    Entity::init(pRenderer, pResourceLoader);
}
void    
HexaTerrain::draw() {
    std::vector<Mesh*>* meshes = mTerrainResource->getMeshes();
    for ( auto it = meshes->begin(); it != meshes->end(); ++it ) {
        mRenderer->drawSolid(*it, mWorldTransform, mTerrainMaterial);
    }
}
HexaTerrain::~HexaTerrain() {
}