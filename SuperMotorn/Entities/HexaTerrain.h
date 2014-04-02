#pragma once
#include "Entity.h"
class HexaTerrainResource;

class HexaTerrain : public Entity {
    std::wstring            mTerrainName;
    HexaTerrainResource*    mTerrainResource;
    Material*               mTerrainMaterial;
    Renderer*               mRenderer;
public:
                    HexaTerrain(const std::wstring& pTerrainName);
    virtual void    init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    virtual void    draw();
                    ~HexaTerrain();
};

