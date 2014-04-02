#include "HexaTerrainResource.h"
#include "Mesh.h"
#include "tinyxml2.h"
#include <iostream>
#include <sstream>
HexaTerrainResource::HexaTerrainResource(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools) : AbstractResource(pFileName, pTimeStamp, pLoadingTools) {
}
void
HexaTerrainResource::load() {
    using namespace std;
    using namespace tinyxml2;
    tinyxml2::XMLDocument terrain;
    terrain.LoadFile(string(mFileName.begin(), mFileName.end()).c_str());
    if ( terrain.ErrorID() == 0 ) {
        mPositions.clear();
        mNormals.clear();
        mTex0.clear();
        mPolyList.clear();
        XMLElement* geometry = terrain.FirstChildElement("hexaTerrain");
        float radius;
        float distance;
        float slope;
        geometry->QueryFloatAttribute("radius", &radius);
        geometry->QueryFloatAttribute("scale", &mScale);
        mScale = 1 / mScale;
        geometry->QueryFloatAttribute("slope", &slope);
        geometry->QueryFloatAttribute("distance", &distance);
        mMaterial = geometry->FirstChildElement("material")->GetText();
        vector<vector<Hexagon> > hexaGrid;
        vector<Vector3> cornerOffsets;
        float sqrt3over2 = sqrt(3) / 2;
        //Outer vertices
        cornerOffsets.push_back(Vector3(0.5f, 0.0f, sqrt3over2)*radius);
        cornerOffsets.push_back(Vector3(1.0f, 0.0f, 0.0f)*radius);
        cornerOffsets.push_back(Vector3(0.5f, 0.0f, -sqrt3over2)*radius);
        cornerOffsets.push_back(Vector3(-0.5f, 0.0f, -sqrt3over2)*radius);
        cornerOffsets.push_back(Vector3(-1.0f, 0.0f, 0.0f)*radius);
        cornerOffsets.push_back(Vector3(-0.5f, 0.0f, sqrt3over2)*radius);
        //Inner
        float slopeFactor = (radius / (radius + slope));
        cornerOffsets.push_back(cornerOffsets[0]*slopeFactor);
        cornerOffsets.push_back(cornerOffsets[1]*slopeFactor);
        cornerOffsets.push_back(cornerOffsets[2]*slopeFactor);
        cornerOffsets.push_back(cornerOffsets[3]*slopeFactor);
        cornerOffsets.push_back(cornerOffsets[4]*slopeFactor);
        cornerOffsets.push_back(cornerOffsets[5]*slopeFactor);
        for ( const XMLElement* hexagon = geometry->FirstChildElement("tile"); hexagon != NULL; hexagon = hexagon->NextSiblingElement() ) {
            Vector3 position(hexagon->FirstChildElement("position"));
            float zoffset = (((int)position.getX()) % 2) * (sqrt3over2*radius + distance/2);
            //float xoffset = (((int)position.getX()) % 2) * ;
            Vector3 p = Vector3(
                position.getX() * (radius*1.5f + distance*sqrt3over2), 
                position.getY(), 
                position.getZ() * (sqrt3over2 * radius*2 + distance) + zoffset);
            Hexagon h;
            for ( int i = 0; i < h.positions.size(); ++i ) {
                h.positions[i] = p + cornerOffsets[i];
            }
            hexaGrid.resize(max(hexaGrid.size(), (int)position.getX()+1));
            hexaGrid[(int)position.getX()].resize(max(hexaGrid[(int)position.getX()].size(), (int)position.getZ()+1));
            hexaGrid[(int)position.getX()][(int)position.getZ()] = h;
        }
        for ( int x = 0; x < hexaGrid.size(); x++ ) {
            for ( int z = 0; z < hexaGrid[x].size(); z++ ) {
                if ( z + 1 < hexaGrid[x].size() ) {
                    hexaGrid[x][z].neighbours[0] = &hexaGrid[x][z + 1];
                }
                if ( z - 1 > 0 ) {
                    hexaGrid[x][z].neighbours[3] = &hexaGrid[x][z - 1];
                }
                if ( x % 2 == 1 ) {
                    if ( x + 1 < hexaGrid.size() && z < hexaGrid[x + 1].size() ) {
                        hexaGrid[x][z].neighbours[1] = &hexaGrid[x + 1][z];
                    }
                    if ( x + 1 < hexaGrid.size() && z + 1 < hexaGrid[x + 1].size() ) {
                        hexaGrid[x][z].neighbours[2] = &hexaGrid[x + 1][z+1];
                    }
                    if ( x - 1 > 0 && z < hexaGrid[x - 1].size() ) {
                        hexaGrid[x][z].neighbours[4] = &hexaGrid[x - 1][z];
                    }
                    if ( x - 1 > 0 && z + 1 < hexaGrid[x - 1].size() ) {
                        hexaGrid[x][z].neighbours[5] = &hexaGrid[x - 1][z+1];
                    }
                } else {
                    if ( x + 1 < hexaGrid.size() && z > 0 ) {
                    }
                    if ( x + 1 < hexaGrid.size() && z  < hexaGrid[x + 1].size() ) {
                    }
                    if ( x + 1 < hexaGrid.size() && z > 0 && z < hexaGrid[x + 1].size()) {
                    }
                }
                addFace(hexaGrid[x][z].positions[6], hexaGrid[x][z].positions[10], hexaGrid[x][z].positions[11]);
                addFace(hexaGrid[x][z].positions[6], hexaGrid[x][z].positions[9], hexaGrid[x][z].positions[10]);
                addFace(hexaGrid[x][z].positions[6], hexaGrid[x][z].positions[7], hexaGrid[x][z].positions[9]);
                addFace(hexaGrid[x][z].positions[7], hexaGrid[x][z].positions[8], hexaGrid[x][z].positions[9]);
                vector<Vector3> slopeNormals;
                slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));
                slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));
                slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));
                slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));
                slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));
                slopeNormals.push_back(Vector3(0.0f, 1.0f, 0.0f));

                if ( x % 2 == 1 ) {
                    if ( x + 1 < hexaGrid.size() && z < hexaGrid[x + 1].size() ) {
                        addFace(hexaGrid[x][z].positions[1], hexaGrid[x + 1][z].positions[5], hexaGrid[x + 1][z].positions[4]);
                        addFace(hexaGrid[x][z].positions[1], hexaGrid[x + 1][z].positions[4], hexaGrid[x][z].positions[2]);
                        //slopeNormals[5] = Vector3(hexaGrid[x][z].positions[2] - hexaGrid[x][z].positions[1]).cross(Vector3(hexaGrid[x][z].positions[2] - hexaGrid[x + 1][z].positions[5])).normalized();
                    }
                    if ( x + 1 < hexaGrid.size() && z + 1 < hexaGrid[x + 1].size() ) {
                        addFace(hexaGrid[x + 1][z + 1].positions[4], hexaGrid[x + 1][z + 1].positions[3], hexaGrid[x][z].positions[0]);
                        addFace(hexaGrid[x][z].positions[0], hexaGrid[x + 1][z + 1].positions[3], hexaGrid[x][z].positions[1]);
                    }
                    if ( x + 1 < hexaGrid.size() && z + 1 < hexaGrid[x + 1].size() && z + 1 < hexaGrid[x].size() ) {
                        addFace(hexaGrid[x][z].positions[1], hexaGrid[x + 1][z + 1].positions[3], hexaGrid[x + 1][z].positions[5]);
                        addFace(hexaGrid[x][z+1].positions[2], hexaGrid[x + 1][z + 1].positions[4], hexaGrid[x][z].positions[0]);
                    }
                    
                } else {
                    if ( x + 1 < hexaGrid.size() && z > 0 ) {
                        addFace(hexaGrid[x + 1][z - 1].positions[5], hexaGrid[x + 1][z - 1].positions[4], hexaGrid[x][z].positions[2]);
                        addFace(hexaGrid[x][z].positions[1], hexaGrid[x + 1][z - 1].positions[5], hexaGrid[x][z].positions[2]);
                        //slopeNormals[2] = Vector3(hexaGrid[x][z].positions[1] - hexaGrid[x][z].positions[2]).cross(Vector3(hexaGrid[x][z].positions[1] - hexaGrid[x + 1][z -1].positions[4])).normalized();
                    }
                    if ( x + 1 < hexaGrid.size() && z  < hexaGrid[x + 1].size() ) {
                        addFace(hexaGrid[x + 1][z].positions[4], hexaGrid[x + 1][z].positions[3], hexaGrid[x][z].positions[1]);
                        addFace(hexaGrid[x][z].positions[0], hexaGrid[x + 1][z].positions[4], hexaGrid[x][z].positions[1]);
                    }
                    if ( x + 1 < hexaGrid.size() && z > 0 && z < hexaGrid[x + 1].size()) {
                        addFace(hexaGrid[x + 1][z].positions[3], hexaGrid[x + 1][z - 1].positions[5], hexaGrid[x][z].positions[1]);
                        addFace(hexaGrid[x][z - 1].positions[0], hexaGrid[x][z].positions[2], hexaGrid[x + 1][z - 1].positions[4]);
                    }
                }
                if ( z + 1 < hexaGrid[x].size()) {
                    addFace(hexaGrid[x][z].positions[5], hexaGrid[x][z + 1].positions[3], hexaGrid[x][z].positions[0]);
                    addFace(hexaGrid[x][z].positions[0], hexaGrid[x][z + 1].positions[3], hexaGrid[x][z + 1].positions[2]);
                    slopeNormals[0] = Vector3(hexaGrid[x][z].positions[5] - hexaGrid[x][z + 1].positions[3]).cross(Vector3(hexaGrid[x][z].positions[5] - hexaGrid[x][z].positions[0])).normalized();
                }
                if ( z - 1 > 0 ) {
                    slopeNormals[3] = Vector3(hexaGrid[x][z].positions[3] - hexaGrid[x][z].positions[2]).cross(Vector3(hexaGrid[x][z].positions[3] - hexaGrid[x][z -1].positions[0])).normalized();
                }
                addFace(hexaGrid[x][z].positions[5], hexaGrid[x][z].positions[0], hexaGrid[x][z].positions[11], slopeNormals[0]);
                addFace(hexaGrid[x][z].positions[11], hexaGrid[x][z].positions[0], hexaGrid[x][z].positions[6], slopeNormals[0]);
                addFace(hexaGrid[x][z].positions[6], hexaGrid[x][z].positions[0], hexaGrid[x][z].positions[1], slopeNormals[1]);
                addFace(hexaGrid[x][z].positions[1], hexaGrid[x][z].positions[7], hexaGrid[x][z].positions[6], slopeNormals[1]);
                addFace(hexaGrid[x][z].positions[7], hexaGrid[x][z].positions[1], hexaGrid[x][z].positions[8], slopeNormals[2]);
                addFace(hexaGrid[x][z].positions[8], hexaGrid[x][z].positions[1], hexaGrid[x][z].positions[2], slopeNormals[2]);
                addFace(hexaGrid[x][z].positions[9], hexaGrid[x][z].positions[8], hexaGrid[x][z].positions[3], slopeNormals[3]);
                addFace(hexaGrid[x][z].positions[3], hexaGrid[x][z].positions[8], hexaGrid[x][z].positions[2], slopeNormals[3]);
                addFace(hexaGrid[x][z].positions[4], hexaGrid[x][z].positions[10], hexaGrid[x][z].positions[9], slopeNormals[4]);
                addFace(hexaGrid[x][z].positions[9], hexaGrid[x][z].positions[3], hexaGrid[x][z].positions[4], slopeNormals[4]);
                addFace(hexaGrid[x][z].positions[4], hexaGrid[x][z].positions[5], hexaGrid[x][z].positions[10], slopeNormals[5]);
                addFace(hexaGrid[x][z].positions[10], hexaGrid[x][z].positions[5], hexaGrid[x][z].positions[11], slopeNormals[5]);
            }
        }
        
        mMeshes.clear(); // TODO LEAK
        mMeshes.push_back(new Mesh(mPositions, mNormals, mTex0, mPolyList, mLoadingTools));
    }
}
void
HexaTerrainResource::addFace(Vector3 pPoint1, Vector3 pPoint2, Vector3 pPoint3) {
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
    mPolyList.push_back(mTex0.size() - 3);
    mPolyList.push_back(mPositions.size() - 2);
    mPolyList.push_back(mNormals.size() - 1);
    mPolyList.push_back(mTex0.size() - 2);
    mPolyList.push_back(mPositions.size() - 1);
    mPolyList.push_back(mNormals.size() - 1);
    mPolyList.push_back(mTex0.size() - 1);
}
void
HexaTerrainResource::addFace(Vector3 pPoint1, Vector3 pPoint2, Vector3 pPoint3, Vector3 pSlopeNormal) {
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
    mPolyList.push_back(mTex0.size() - 3);
    mPolyList.push_back(mPositions.size() - 2);
    mPolyList.push_back(mNormals.size() - 1);
    mPolyList.push_back(mTex0.size() - 2);
    mPolyList.push_back(mPositions.size() - 1);
    mPolyList.push_back(mNormals.size() - 1);
    mPolyList.push_back(mTex0.size() - 1);
}
std::vector<Mesh*>*  
HexaTerrainResource::getMeshes() {
    return &mMeshes;
}
std::string         
HexaTerrainResource::getMaterial() {
    return mMaterial;
}
HexaTerrainResource::~HexaTerrainResource() {
}
