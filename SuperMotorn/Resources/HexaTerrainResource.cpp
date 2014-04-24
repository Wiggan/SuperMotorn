#include "HexaTerrainResource.h"
#include "Mesh.h"
#include "Material.h"
#include "tinyxml2.h"
#include <iostream>
#include <sstream>
#include "Util.h"

const float sqrt3over2 = sqrt(3) / 2;

HexaTerrainResource::HexaTerrainResource(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools) : AbstractResource(pFileName, pTimeStamp, pLoadingTools), mDebugMesh(NULL) {
    using namespace std;
    using namespace tinyxml2;
    mDocument.LoadFile(string(mFileName.begin(), mFileName.end()).c_str());
}
void
HexaTerrainResource::load() {
    using namespace std;
    using namespace tinyxml2;
    if ( mDocument.ErrorID() == 0 ) {
        if ( mMeshes.size() > 0 ) {
            delete mMeshes.front();
        }
        mMeshes.clear();
        mTransforms.clear();
        mMaterials.clear();
        mPositions.clear();
        mNormals.clear();
        mColors.clear();
        mOctreeBoxes.clear();
        mColors.push_back(Vector3(1.0f, 0.0f, 0.0f));
        mColors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        mColors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        mTex0.clear();
        mPolyList.clear();
        XMLElement* geometry = mDocument.FirstChildElement("hexaTerrain");
        float slope;
        geometry->QueryFloatAttribute("radius", &mRadius);
        geometry->QueryFloatAttribute("scale", &mScale);
        mScale = 1 / mScale;
        geometry->QueryFloatAttribute("slope", &slope);
        geometry->QueryFloatAttribute("increment", &mIncrement);
        geometry->QueryFloatAttribute("distance", &mDistance);
        mMaterial = geometry->FirstChildElement("material")->GetText();
        vector<vector<Hexagon*> > hexaGrid;
        vector<Vector3> cornerOffsets;
        XMLElement* offset = mDocument.FirstChildElement("hexaTerrain")->FirstChildElement("offset");
        offset->QueryIntAttribute("x", &mXOffset);
        offset->QueryIntAttribute("z", &mZOffset);
        //Outer vertices
        cornerOffsets.push_back(Vector3(0.5f, 0.0f, sqrt3over2)*mRadius);
        cornerOffsets.push_back(Vector3(1.0f, 0.0f, 0.0f)*mRadius);
        cornerOffsets.push_back(Vector3(0.5f, 0.0f, -sqrt3over2)*mRadius);
        cornerOffsets.push_back(Vector3(-0.5f, 0.0f, -sqrt3over2)*mRadius);
        cornerOffsets.push_back(Vector3(-1.0f, 0.0f, 0.0f)*mRadius);
        cornerOffsets.push_back(Vector3(-0.5f, 0.0f, sqrt3over2)*mRadius);
        //Inner
        float slopeFactor = (mRadius / (mRadius + slope));
        cornerOffsets.push_back(cornerOffsets[0]*slopeFactor);
        cornerOffsets.push_back(cornerOffsets[1]*slopeFactor);
        cornerOffsets.push_back(cornerOffsets[2]*slopeFactor);
        cornerOffsets.push_back(cornerOffsets[3]*slopeFactor);
        cornerOffsets.push_back(cornerOffsets[4]*slopeFactor);
        cornerOffsets.push_back(cornerOffsets[5]*slopeFactor);
        for ( const XMLElement* hexagon = geometry->FirstChildElement("tile"); hexagon != NULL; hexagon = hexagon->NextSiblingElement() ) {
            Vector3 position = Vector3(hexagon->FirstChildElement("position"));
            Vector3 p = hexa2local(position);
            Hexagon* h = new Hexagon;
            for ( int i = 0; i < h->positions.size(); ++i ) {
                h->positions[i] = p + cornerOffsets[i];
            }
            hexaGrid.resize(max(hexaGrid.size(), (int)position.getX()+2));
            hexaGrid[(int)position.getX()].resize(max(hexaGrid[(int)position.getX()].size(), (int)position.getZ()+2));
            hexaGrid[(int)position.getX()][(int)position.getZ()] = h;
            const XMLElement* texture = hexagon->FirstChildElement("texture");
            if ( texture ) {
                h->color = texture->IntAttribute("layer");
            }
            const XMLElement* mesh = hexagon->FirstChildElement("mesh");
            const XMLElement* material = hexagon->FirstChildElement("material");
            if ( mesh && material ) {
                mMeshes.push_back((Mesh*)mLoadingTools->getMesh(Util::string2wstring(mesh->GetText())));
                mMaterials.push_back((Material*)mLoadingTools->getMaterial(Util::string2wstring(material->GetText())));
                Vector3 rotation(hexagon->FirstChildElement("rotation"));
                Vector3 scale(hexagon->FirstChildElement("scale"), Vector3(1.0f, 1.0f, 1.0f));
                mTransforms.push_back(Matrix(scale, rotation, p + Vector3(mXOffset, 0.0f, mZOffset)));
            }
        }
        for ( int x = 0; x < hexaGrid.size(); x++ ) {
            for ( int z = 0; z < hexaGrid[x].size(); z++ ) {
                Hexagon* currentHexa = hexaGrid[x][z];
                if ( currentHexa ) {
                    if ( z + 1 < hexaGrid[x].size() ) {
                        currentHexa->neighbours[0] = hexaGrid[x][z + 1];
                    }
                    if ( z > 0 ) {
                        currentHexa->neighbours[3] = hexaGrid[x][z - 1];
                    }
                    if ( x % 2 == 1 ) {
                        if ( x + 1 < hexaGrid.size() && z < hexaGrid[x + 1].size() ) {
                            currentHexa->neighbours[2] = hexaGrid[x + 1][z];
                        }
                        if ( x + 1 < hexaGrid.size() && z + 1 < hexaGrid[x + 1].size() ) {
                            currentHexa->neighbours[1] = hexaGrid[x + 1][z + 1];
                        }
                        if ( z < hexaGrid[x + 1].size() ) {
                            currentHexa->neighbours[4] = hexaGrid[x + 1][z];
                        }
                        if ( x > 0 && z + 1 < hexaGrid[x - 1].size() ) {
                            currentHexa->neighbours[5] = hexaGrid[x - 1][z + 1];
                        }
                    } else {
                        if ( x + 1 < hexaGrid.size() && z < hexaGrid[x + 1].size() ) {
                            currentHexa->neighbours[1] = hexaGrid[x + 1][z];
                        }
                        if ( x + 1 < hexaGrid.size() && z > 0 && z - 1 < hexaGrid[x + 1].size() ) {
                            currentHexa->neighbours[2] = hexaGrid[x + 1][z - 1];
                        }
                        if ( x > 0 && z < hexaGrid[x - 1].size() ) {
                            currentHexa->neighbours[5] = hexaGrid[x - 1][z];
                        }
                        if ( z > 0 && z - 1 < hexaGrid[x + 1].size() ) {
                            currentHexa->neighbours[4] = hexaGrid[x + 1][z - 1];
                        }
                    }
                    addFace(currentHexa->positions[6], currentHexa->positions[10], currentHexa->positions[11], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[6], currentHexa->positions[9], currentHexa->positions[10], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[6], currentHexa->positions[7], currentHexa->positions[9], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[7], currentHexa->positions[8], currentHexa->positions[9], currentHexa->color, currentHexa->color, currentHexa->color);
                    vector<Vector3> slopeNormals;
                    slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));
                    slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));
                    slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));
                    slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));
                    slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));
                    slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));




                    if ( currentHexa->neighbours[2] ) {
                        addFace(currentHexa->neighbours[2]->positions[5], currentHexa->neighbours[2]->positions[4], currentHexa->positions[2], currentHexa->neighbours[2]->color, currentHexa->neighbours[2]->color, currentHexa->color);
                        addFace(currentHexa->positions[1], currentHexa->neighbours[2]->positions[5], currentHexa->positions[2], currentHexa->color, currentHexa->neighbours[2]->color, currentHexa->color);
                        slopeNormals[2] = Vector3(currentHexa->positions[2] - currentHexa->positions[1]).cross(Vector3(currentHexa->positions[2] - currentHexa->neighbours[2]->positions[4])).normalized();
                    }
                    if ( currentHexa->neighbours[1] ) {
                        addFace(currentHexa->neighbours[1]->positions[4], currentHexa->neighbours[1]->positions[3], currentHexa->positions[1], currentHexa->neighbours[1]->color, currentHexa->neighbours[1]->color, currentHexa->color);
                        addFace(currentHexa->positions[0], currentHexa->neighbours[1]->positions[4], currentHexa->positions[1], currentHexa->color, currentHexa->neighbours[1]->color, currentHexa->color);
                        slopeNormals[1] = Vector3(currentHexa->positions[1] - currentHexa->positions[0]).cross(Vector3(currentHexa->positions[1] - currentHexa->neighbours[1]->positions[4])).normalized();
                    }
                    if ( currentHexa->neighbours[1] && currentHexa->neighbours[2] ) {
                        addFace(currentHexa->neighbours[1]->positions[3], currentHexa->neighbours[2]->positions[5], currentHexa->positions[1], currentHexa->neighbours[1]->color, currentHexa->neighbours[2]->color, currentHexa->color);
                    }
                    if ( currentHexa->neighbours[2] && currentHexa->neighbours[3] ) {
                        addFace(currentHexa->neighbours[3]->positions[0], currentHexa->positions[2], currentHexa->neighbours[2]->positions[4], currentHexa->neighbours[3]->color, currentHexa->color, currentHexa->neighbours[2]->color);
                    }
                    if ( currentHexa->neighbours[0] ) {
                        addFace(currentHexa->positions[5], currentHexa->neighbours[0]->positions[3], currentHexa->positions[0], currentHexa->color, currentHexa->neighbours[0]->color, currentHexa->color);
                        addFace(currentHexa->positions[0], currentHexa->neighbours[0]->positions[3], currentHexa->neighbours[0]->positions[2], currentHexa->color, currentHexa->neighbours[0]->color, currentHexa->neighbours[0]->color);
                        slopeNormals[0] = Vector3(currentHexa->positions[5] - currentHexa->neighbours[0]->positions[3]).cross(Vector3(currentHexa->positions[5] - currentHexa->positions[0])).normalized();
                    }
                    if ( currentHexa->neighbours[3] ) {
                        slopeNormals[3] = Vector3(currentHexa->positions[3] - currentHexa->positions[2]).cross(Vector3(currentHexa->positions[3] - currentHexa->neighbours[3]->positions[0])).normalized();
                    }
                    if ( currentHexa->neighbours[4] ) {
                        slopeNormals[4] = Vector3(currentHexa->positions[3] - currentHexa->positions[4]).cross(Vector3(currentHexa->positions[3] - currentHexa->neighbours[4]->positions[1])).normalized();
                    }
                    if ( currentHexa->neighbours[5] ) {
                        slopeNormals[5] = Vector3(currentHexa->positions[5] - currentHexa->positions[4]).cross(Vector3(currentHexa->positions[5] - currentHexa->neighbours[5]->positions[1])).normalized();
                    }
                    addFace(currentHexa->positions[5], currentHexa->positions[0], currentHexa->positions[11], slopeNormals[0], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[11], currentHexa->positions[0], currentHexa->positions[6], slopeNormals[0], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[6], currentHexa->positions[0], currentHexa->positions[1], slopeNormals[1], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[1], currentHexa->positions[7], currentHexa->positions[6], slopeNormals[1], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[7], currentHexa->positions[1], currentHexa->positions[8], slopeNormals[2], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[8], currentHexa->positions[1], currentHexa->positions[2], slopeNormals[2], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[9], currentHexa->positions[8], currentHexa->positions[3], slopeNormals[3], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[3], currentHexa->positions[8], currentHexa->positions[2], slopeNormals[3], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[4], currentHexa->positions[10], currentHexa->positions[9], slopeNormals[4], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[9], currentHexa->positions[3], currentHexa->positions[4], slopeNormals[4], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[4], currentHexa->positions[5], currentHexa->positions[10], slopeNormals[5], currentHexa->color, currentHexa->color, currentHexa->color);
                    addFace(currentHexa->positions[10], currentHexa->positions[5], currentHexa->positions[11], slopeNormals[5], currentHexa->color, currentHexa->color, currentHexa->color);
                }
            }
        }
        Mesh* mesh = new Mesh(mPositions, mNormals, mColors, mTex0, mPolyList, mLoadingTools);
        mMeshes.insert(mMeshes.begin(), mesh);
        mTransforms.insert(mTransforms.begin(), Matrix(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(mXOffset, 0.0f, mZOffset)));
        mMaterials.insert(mMaterials.begin(), (Material*)mLoadingTools->getMaterial(Util::string2wstring(mMaterial)));
        putFacesInOctrees(mesh);
    }
}
void                
HexaTerrainResource::putFacesInOctrees(Mesh* pMesh) {
    int faceCount = pMesh->getIndices()->size();
    int i1;
    int i2;
    int i3;
    for ( int i = 0; i < faceCount; i++ ) {
        if ( i % 3 == 0 ) {
            i1 = (*pMesh->getIndices())[i];
            i2 = (*pMesh->getIndices())[i + 1];
            i3 = (*pMesh->getIndices())[i + 2];
        }
        Vector3 point = (*pMesh->getPositions())[(*pMesh->getIndices())[i]];
        bool contained = false;
        HexagonOctreeBox* box = NULL;
        for ( auto it = mOctreeBoxes.begin(); it != mOctreeBoxes.end(); ++it ) {
            if ( (*it)->box.Contains(point) ) {
                contained = true;
                box = *it;
                break;
            }
        }
        if ( !contained ) {
            box = new HexagonOctreeBox;
            mOctreeBoxes.push_back(box);
        }
        int boxExtent = 10;
        box->box = DirectX::BoundingBox(Vector3((((int)point.getX()+boxExtent/2) / boxExtent) * boxExtent, 0.0f, (((int)point.getZ()+boxExtent/2) / boxExtent) * boxExtent), Vector3(boxExtent/2, 1000.0f, boxExtent/2));
        box->indices.push_back(i1);
        box->indices.push_back(i2);
        box->indices.push_back(i3);
    }
}
void
HexaTerrainResource::addFace(Vector3 pPoint1, Vector3 pPoint2, Vector3 pPoint3, int pP1Color, int pP2Color, int pP3Color) {
    mPositions.push_back(pPoint1);
    mPositions.push_back(pPoint2);
    mPositions.push_back(pPoint3);
    mTex0.push_back(DirectX::XMFLOAT2(pPoint1.getX()*mScale, pPoint1.getZ()*mScale));
    mTex0.push_back(DirectX::XMFLOAT2(pPoint2.getX()*mScale, pPoint2.getZ()*mScale));
    mTex0.push_back(DirectX::XMFLOAT2(pPoint3.getX()*mScale, pPoint3.getZ()*mScale));
    mNormals.push_back(Vector3(pPoint1 - pPoint2).cross(
        Vector3(pPoint1 - pPoint3)).normalized());
    mPolyList.push_back(mPositions.size() - 3);
    mPolyList.push_back(mNormals.size() - 1);
    mPolyList.push_back(pP1Color);
    mPolyList.push_back(mTex0.size() - 3);
    mPolyList.push_back(mPositions.size() - 2);
    mPolyList.push_back(mNormals.size() - 1);
    mPolyList.push_back(pP2Color);
    mPolyList.push_back(mTex0.size() - 2);
    mPolyList.push_back(mPositions.size() - 1);
    mPolyList.push_back(mNormals.size() - 1);
    mPolyList.push_back(pP3Color);
    mPolyList.push_back(mTex0.size() - 1);
}
void
HexaTerrainResource::addFace(Vector3 pPoint1, Vector3 pPoint2, Vector3 pPoint3, Vector3 pSlopeNormal, int pP1Color, int pP2Color, int pP3Color) {
    mPositions.push_back(pPoint1);
    mPositions.push_back(pPoint2);
    mPositions.push_back(pPoint3);
    mTex0.push_back(DirectX::XMFLOAT2(pPoint1.getX()*mScale, pPoint1.getZ()*mScale));
    mTex0.push_back(DirectX::XMFLOAT2(pPoint2.getX()*mScale, pPoint2.getZ()*mScale));
    mTex0.push_back(DirectX::XMFLOAT2(pPoint3.getX()*mScale, pPoint3.getZ()*mScale));
    mNormals.push_back((Vector3(pPoint1 - pPoint2).cross(
        Vector3(pPoint1 - pPoint3)).normalized() + pSlopeNormal).normalized());
    mPolyList.push_back(mPositions.size() - 3);
    mPolyList.push_back(mNormals.size() - 1);
    mPolyList.push_back(pP1Color);
    mPolyList.push_back(mTex0.size() - 3);
    mPolyList.push_back(mPositions.size() - 2);
    mPolyList.push_back(mNormals.size() - 1);
    mPolyList.push_back(pP2Color);
    mPolyList.push_back(mTex0.size() - 2);
    mPolyList.push_back(mPositions.size() - 1);
    mPolyList.push_back(mNormals.size() - 1);
    mPolyList.push_back(pP3Color);
    mPolyList.push_back(mTex0.size() - 1);
}
std::vector<Mesh*>*  
HexaTerrainResource::getMeshes() {
    return &mMeshes;
}
std::vector<Material*>*  
HexaTerrainResource::getMaterials() {
    return &mMaterials;
}
std::vector<Matrix>*  
HexaTerrainResource::getTransforms() {
    return &mTransforms;
}
float               
HexaTerrainResource::getRadius() {
    return mRadius;
}
float               
HexaTerrainResource::getDistance() {
    return mDistance;
}
float               
HexaTerrainResource::getIncrement() {
    return mIncrement;
}
std::vector<HexagonOctreeBox*>*  
HexaTerrainResource::getOctreeBoxes() {
    return &mOctreeBoxes;
}
Mesh*               
HexaTerrainResource::getDebugMesh() {
    if ( mDebugMesh == NULL ) {
        float sqrt3over2 = sqrt(3) / 2;
        std::vector<DirectX::XMFLOAT3> normals;
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        std::vector<DirectX::XMFLOAT3> positions;
        positions.push_back(Vector3(0.5f, 0.01f, sqrt3over2)*mRadius);
        positions.push_back(Vector3(1.0f, 0.01f, 0.0f)*mRadius);
        positions.push_back(Vector3(0.5f, 0.01f, -sqrt3over2)*mRadius);
        positions.push_back(Vector3(-0.5f, 0.01f, -sqrt3over2)*mRadius);
        positions.push_back(Vector3(-1.0f, 0.01f, 0.0f)*mRadius);
        positions.push_back(Vector3(-0.5f, 0.01f, sqrt3over2)*mRadius);
        
        std::vector<DirectX::XMFLOAT2> tex;
        tex.push_back(DirectX::XMFLOAT2(positions[0].x*mScale, positions[0].z*mScale));
        tex.push_back(DirectX::XMFLOAT2(positions[1].x*mScale, positions[1].z*mScale));
        tex.push_back(DirectX::XMFLOAT2(positions[2].x*mScale, positions[2].z*mScale));
        tex.push_back(DirectX::XMFLOAT2(positions[3].x*mScale, positions[3].z*mScale));
        tex.push_back(DirectX::XMFLOAT2(positions[4].x*mScale, positions[4].z*mScale));
        tex.push_back(DirectX::XMFLOAT2(positions[5].x*mScale, positions[5].z*mScale));

        std::vector<int> polyList;
        polyList.push_back(0);
        polyList.push_back(0);
        polyList.push_back(0);
        polyList.push_back(1);
        polyList.push_back(0);
        polyList.push_back(1);
        polyList.push_back(5);
        polyList.push_back(0);
        polyList.push_back(5);

        polyList.push_back(1);
        polyList.push_back(0);
        polyList.push_back(1);
        polyList.push_back(2);
        polyList.push_back(0);
        polyList.push_back(2);
        polyList.push_back(3);
        polyList.push_back(0);
        polyList.push_back(3);

        polyList.push_back(3);
        polyList.push_back(0);
        polyList.push_back(3);
        polyList.push_back(4);
        polyList.push_back(0);
        polyList.push_back(4);
        polyList.push_back(5);
        polyList.push_back(0);
        polyList.push_back(5);

        polyList.push_back(5);
        polyList.push_back(0);
        polyList.push_back(5);
        polyList.push_back(1);
        polyList.push_back(0);
        polyList.push_back(1);
        polyList.push_back(3);
        polyList.push_back(0);
        polyList.push_back(3);
        mDebugMesh = new Mesh(positions, normals, tex, polyList, mLoadingTools);
    }
    return mDebugMesh;
}
Vector3
HexaTerrainResource::hexa2local(Vector3 pHexaPosition) {
    float zoffset = (((int)round(pHexaPosition.getX())) % 2) * (sqrt3over2*mRadius + mDistance/2);
    Vector3 p = Vector3(
        round(pHexaPosition.getX()) * (mRadius*1.5f + mDistance*sqrt3over2), 
        round(pHexaPosition.getY()) * mIncrement, 
        round(pHexaPosition.getZ()) * (sqrt3over2 * mRadius*2 + mDistance) + zoffset);
    return p;
}
Matrix              
HexaTerrainResource::getTransformFromHexaCoord(Vector3 pHexaPos) {
    Vector3 pos = hexa2local(pHexaPos);
    pos.setY(pHexaPos.getY());
    return Matrix(Vector3(1.0f, 1.0f, 1.0f), Vector3(), pos);
}
Vector3
HexaTerrainResource::getHexaCoordFromLocalCoord(Vector3 pLocalCoord) {
    Vector3 p;
    p.setX(round(pLocalCoord.getX() / (mRadius*1.5f + mDistance*sqrt3over2)));
    float zoffset = (((int)p.getX()) % 2) * (sqrt3over2*mRadius + mDistance / 2);
    p.setZ(round((pLocalCoord.getZ() - zoffset) / (sqrt3over2 * mRadius * 2 + mDistance)));
    p.setY(pLocalCoord.getY());
    return p;
}
void                
HexaTerrainResource::setHexagonY(Vector3 pHexaPos) {
    using namespace std;
    using namespace tinyxml2;
    if ( mDocument.ErrorID() == 0 ) {
        XMLElement* child = mDocument.FirstChildElement("hexaTerrain")->FirstChildElement("tile");
        int x;
        int z;
        for( child; child; child=child->NextSiblingElement() )
	    {
            XMLElement* position = child->FirstChildElement("position");
            position->QueryIntAttribute("x", &x);
            position->QueryIntAttribute("z", &z);
            if ( x == (int)pHexaPos.getX() && z == (int)pHexaPos.getZ() ) {
                position->SetAttribute("y", round(pHexaPos.getY()));
                break;
            }
	    }
        load();
    }
}
void                
HexaTerrainResource::addHexagon(Vector3 pHexaPos) {
    if ( pHexaPos.getX() < 0 || pHexaPos.getZ() < 0 ) {
        enlarge();
        pHexaPos.setX(pHexaPos.getX()+10);
        pHexaPos.setZ(pHexaPos.getZ()+10);
    }
    using namespace std;
    using namespace tinyxml2;
    if ( mDocument.ErrorID() == 0 ) {
        XMLNode* tile = mDocument.NewElement("tile");
        XMLElement* position = mDocument.NewElement("position");
        position->SetAttribute("x", round(pHexaPos.getX()));
        position->SetAttribute("y", pHexaPos.getY());
        position->SetAttribute("z", round(pHexaPos.getZ()));
        tile->LinkEndChild(position);
        mDocument.FirstChildElement("hexaTerrain")->LinkEndChild(tile);
        load();
    }
}
void                
HexaTerrainResource::removeHexagon(Vector3 pHexaPos) {
    using namespace std;
    using namespace tinyxml2;
    if ( mDocument.ErrorID() == 0 ) {
        XMLElement* child = mDocument.FirstChildElement("hexaTerrain")->FirstChildElement("tile");
        int x;
        int z;
        for( child; child; child=child->NextSiblingElement() )
	    {
            XMLElement* position = child->FirstChildElement("position");
            position->QueryIntAttribute("x", &x);
            position->QueryIntAttribute("z", &z);
            if ( x == (int)pHexaPos.getX() && z == (int)pHexaPos.getZ() ) {
                mDocument.FirstChildElement("hexaTerrain")->DeleteChild(child);
                break;
            }
	    }
        load();
    }
}
void                    
HexaTerrainResource::addMesh(Vector3 pHexaPos, Mesh* pMesh, Vector3 pScale, Vector3 pRotation) {
    using namespace std;
    using namespace tinyxml2;
    if ( mDocument.ErrorID() == 0 ) {
        XMLElement* child = mDocument.FirstChildElement("hexaTerrain")->FirstChildElement("tile");
        int x;
        int z;
        for( child; child; child=child->NextSiblingElement() ) {
            XMLElement* position = child->FirstChildElement("position");
            position->QueryIntAttribute("x", &x);
            position->QueryIntAttribute("z", &z);
            if ( x == (int)pHexaPos.getX() && z == (int)pHexaPos.getZ() ) {
                XMLElement* mesh = child->FirstChildElement("mesh");
                if ( mesh ) {
                    mesh->SetText(Util::wstring2string(Util::stripPath(pMesh->getFileName())).c_str());
                    XMLElement* mesh = child->FirstChildElement("mesh");
                    child->DeleteChild(child->FirstChildElement("scale"));
                    child->DeleteChild(child->FirstChildElement("rotation"));
                    child->LinkEndChild(pScale.toXml(&mDocument, "scale"));
                    child->LinkEndChild(pRotation.toXml(&mDocument, "rotation"));
                } else {
                    XMLElement* material = mDocument.NewElement("material");
                    material->SetText(mMaterial.c_str());
                    mesh = mDocument.NewElement("mesh");
                    mesh->SetText(Util::wstring2string(Util::stripPath(pMesh->getFileName())).c_str());
                    child->LinkEndChild(mesh);
                    child->LinkEndChild(material);
                    child->LinkEndChild(pScale.toXml(&mDocument, "scale"));
                    child->LinkEndChild(pRotation.toXml(&mDocument, "rotation"));
                }
                break;
            }
	    }
        load();
    }
}
void                    
HexaTerrainResource::removeMesh(Vector3 pHexaPos) {
    using namespace std;
    using namespace tinyxml2;
    if ( mDocument.ErrorID() == 0 ) {
        XMLElement* child = mDocument.FirstChildElement("hexaTerrain")->FirstChildElement("tile");
        int x;
        int z;
        for( child; child; child=child->NextSiblingElement() )
	    {
            XMLElement* position = child->FirstChildElement("position");
            position->QueryIntAttribute("x", &x);
            position->QueryIntAttribute("z", &z);
            if ( x == (int)pHexaPos.getX() && z == (int)pHexaPos.getZ() ) {
                child->DeleteChild(child->FirstChildElement("material"));
                child->DeleteChild(child->FirstChildElement("mesh"));
                child->DeleteChild(child->FirstChildElement("scale"));
                child->DeleteChild(child->FirstChildElement("rotation"));
                break;
            }
	    }
        load();
    }
}
void                    
HexaTerrainResource::setMaterial(Vector3 pHexaPos, Material* pMaterial) {
    using namespace tinyxml2;
    XMLElement* material = getTile(pHexaPos)->FirstChildElement("material");
    if ( material ) {
        std::string filename = Util::wstring2string(Util::stripPath(pMaterial->getFileName()));
        material->SetText(filename.c_str());
        load();
    }
}
void                    
HexaTerrainResource::setTexture(Vector3 pHexaPos, int pTexture) {
    using namespace tinyxml2;
    XMLElement* tile = getTile(pHexaPos);
    XMLElement* texture = tile->FirstChildElement("texture");
    if ( texture ) {
        texture->SetAttribute("layer", pTexture);
    } else {
        texture = mDocument.NewElement("texture");
        texture->SetAttribute("layer", pTexture);
        tile->LinkEndChild(texture);
    }
    load();
}
tinyxml2::XMLElement*   
HexaTerrainResource::getTile(Vector3 pHexaPos) {
    using namespace tinyxml2;
    if ( mDocument.ErrorID() == 0 ) {
        XMLElement* child = mDocument.FirstChildElement("hexaTerrain")->FirstChildElement("tile");
        int x;
        int z;
        for( child; child; child=child->NextSiblingElement() )
	    {
            XMLElement* position = child->FirstChildElement("position");
            position->QueryIntAttribute("x", &x);
            position->QueryIntAttribute("z", &z);
            if ( x == (int)pHexaPos.getX() && z == (int)pHexaPos.getZ() ) {
                return child;
            }
	    }
    }
    return NULL;
}
Mesh*                   
HexaTerrainResource::getMesh(Vector3 pHexaPos) {
    Mesh* mesh = NULL;
    tinyxml2::XMLElement* tile = getTile(pHexaPos);
    if ( tile->FirstChildElement("mesh") ) {
        mesh = (Mesh*)mLoadingTools->getMesh(Util::string2wstring(tile->FirstChildElement("mesh")->GetText()));
    }
    return mesh;
}
bool                
HexaTerrainResource::isOccupied(Vector3 pHexaPos) {
    using namespace std;
    using namespace tinyxml2;
    if ( mDocument.ErrorID() == 0 ) {
        XMLElement* child = mDocument.FirstChildElement("hexaTerrain")->FirstChildElement("tile");
        int x;
        int z;
        for( child; child; child=child->NextSiblingElement() )
	    {
            XMLElement* position = child->FirstChildElement("position");
            position->QueryIntAttribute("x", &x);
            position->QueryIntAttribute("z", &z);
            if ( x == round(pHexaPos.getX()) && z == round(pHexaPos.getZ()) ) {
                return true;
            }
	    }
    }
    return false;
}
void                
HexaTerrainResource::enlarge() {
    using namespace std;
    using namespace tinyxml2;
    if ( mDocument.ErrorID() == 0 ) {
        XMLElement* child = mDocument.FirstChildElement("hexaTerrain")->FirstChildElement("tile");
        int x;
        int z;
        XMLElement* offset = mDocument.FirstChildElement("hexaTerrain")->FirstChildElement("offset");
        offset->QueryIntAttribute("x", &x);
        offset->QueryIntAttribute("z", &z);
        offset->SetAttribute("x", x + 10);
        offset->SetAttribute("z", z + 10);
        for( child; child; child=child->NextSiblingElement() )
	    {
            XMLElement* position = child->FirstChildElement("position");
            position->QueryIntAttribute("x", &x);
            position->QueryIntAttribute("z", &z);
            position->SetAttribute("x", x + 10);
            position->SetAttribute("z", z + 10);

	    }
        load();
    }
}
HexaTerrainResource::~HexaTerrainResource() {
    mDocument.SaveFile(Util::wstring2string(mFileName).c_str());
}
