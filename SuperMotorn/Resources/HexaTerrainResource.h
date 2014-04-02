#pragma once
#include "AbstractResource.h"
#include <vector>
#include "Vector3.h"
class Mesh;
struct Hexagon {
    Hexagon() {
        positions.resize(12);
        neighbours.resize(6, NULL);
    }
    std::vector<Vector3> positions;
    std::vector<Hexagon*> neighbours;
};
class HexaTerrainResource : public AbstractResource {
    std::vector<Mesh*>              mMeshes;
    std::string                     mMaterial;
    float                           mScale;
    std::vector<DirectX::XMFLOAT3>  mPositions;
    std::vector<DirectX::XMFLOAT3>  mNormals;
    std::vector<DirectX::XMFLOAT2>  mTex0;
    std::vector<int>                mPolyList;

    void                addFace(Vector3 pPoint1, Vector3 pPoint2, Vector3 pPoint3);
    void                addFace(Vector3 pPoint1, Vector3 pPoint2, Vector3 pPoint3, Vector3 pSlopeNormal);
public:
                        HexaTerrainResource(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
    std::vector<Mesh*>* getMeshes();
    std::string         getMaterial();
    virtual void        load();
                        ~HexaTerrainResource();
};

