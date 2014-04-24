#pragma once
#include "AbstractResource.h"
#include <vector>
#include "Vector3.h"
#include "tinyxml2.h"
#include <DirectXCollision.h>
class Mesh;
class Material;
struct HexagonOctreeBox {
    DirectX::BoundingBox    box;
    std::vector<int>        indices;
};
struct Hexagon {
    Hexagon() {
        positions.resize(12);
        neighbours.resize(6, NULL);
        color = 0;
    }
    std::vector<Vector3>    positions;
    std::vector<Hexagon*>   neighbours;
    int                     color;
};
class HexaTerrainResource : public AbstractResource {
    std::vector<Mesh*>              mMeshes;
    std::vector<HexagonOctreeBox*>  mOctreeBoxes;
    std::vector<Material*>          mMaterials;
    std::vector<Matrix>             mTransforms;
    Mesh*                           mDebugMesh;
    float                           mRadius;
    float                           mDistance;
    std::string                     mMaterial;
    float                           mScale;
    float                           mIncrement;
    std::vector<DirectX::XMFLOAT3>  mPositions;
    std::vector<DirectX::XMFLOAT3>  mNormals;
    std::vector<DirectX::XMFLOAT3>  mColors;
    std::vector<DirectX::XMFLOAT2>  mTex0;
    std::vector<int>                mPolyList;
    int                             mXOffset;
    int                             mZOffset;
    tinyxml2::XMLDocument           mDocument;

    void                putFacesInOctrees(Mesh* pMesh);
    void                addFace(Vector3 pPoint1, Vector3 pPoint2, Vector3 pPoint3, int pP1Color, int pP2Color, int pP3Color);
    void                addFace(Vector3 pPoint1, Vector3 pPoint2, Vector3 pPoint3, Vector3 pSlopeNormal, int pP1Color, int pP2Color, int pP3Color);
public:
                            HexaTerrainResource(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
    virtual void            load();
    
    void                    addHexagon(Vector3 pHexaPos);
    void                    removeHexagon(Vector3 pHexaPos);
    void                    setHexagonY(Vector3 pHexaPos);
    void                    addMesh(Vector3 pHexaPos, Mesh* pMesh, Vector3 pScale, Vector3 pRotation);
    void                    removeMesh(Vector3 pHexaPos);
    void                    setMaterial(Vector3 pHexaPos, Material* pMaterial);
    void                    setTexture(Vector3 pHexaPos, int pTexture);

    Mesh*                   getMesh(Vector3 pHexaPos);
    tinyxml2::XMLElement*   getTile(Vector3 pHexaPos);
    void                    enlarge();
    bool                    isOccupied(Vector3 pHexaPos);
    Vector3                 hexa2local(Vector3 pHexaPosition);
    Vector3                 getHexaCoordFromLocalCoord(Vector3 pLocalCoord);
    Matrix                  getTransformFromHexaCoord(Vector3 pHexaPos);

    std::vector<Mesh*>*             getMeshes();
    std::vector<HexagonOctreeBox*>*  getOctreeBoxes();
    std::vector<Material*>*         getMaterials();
    std::vector<Matrix>*            getTransforms();
    float                           getRadius();
    float                           getDistance();
    float                           getIncrement();
    Mesh*                           getDebugMesh();
                                    ~HexaTerrainResource();
};

