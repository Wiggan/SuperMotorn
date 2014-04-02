#include "Mesh.h"
#include "tinyxml2.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <DirectXMath.h>
#include "Vertex.h"
#include "Texture.h"
#include "Util.h"
#include "Material.h"
Mesh::Mesh(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools) : AbstractResource(pFileName, pTimeStamp, pLoadingTools) {
}
Mesh::Mesh(std::vector<DirectX::XMFLOAT3> pPositions, std::vector<DirectX::XMFLOAT3> pNormals, std::vector<DirectX::XMFLOAT2> pUV, std::vector<int> pPolyList, LoadingToolsInterface* pLoadingTools) : AbstractResource(L"", TimeStamp(), pLoadingTools) {
    createBuffers(pPositions, pNormals, pUV, pPolyList);
}
void
Mesh::load() {
    using namespace std;
    using namespace tinyxml2;
    tinyxml2::XMLDocument mesh;
    mesh.LoadFile(string(mFileName.begin(), mFileName.end()).c_str());
    if ( mesh.ErrorID() == 0 ) {

        XMLElement* geometry = mesh.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->FirstChildElement("geometry");
        vector<DirectX::XMFLOAT3> positions;
        vector<DirectX::XMFLOAT3> normals;
        vector<DirectX::XMFLOAT2> tex0;
        for ( const XMLElement* source = geometry->FirstChildElement("mesh")->FirstChildElement("source"); source != NULL; source = source->NextSiblingElement() ) {
            if ( source->Attribute("id") != NULL ) {
                string id = string(source->Attribute("id"));
                if ( id.find("positions") != string::npos ) {
                    string positionString = source->FirstChildElement("float_array")->GetText();
                    istringstream iss(positionString);
                    vector<string> tokens;
                    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
                    for ( int j = 0; j < tokens.size(); j += 3 ) {
                        positions.push_back(DirectX::XMFLOAT3(atof(tokens[j].c_str()), atof(tokens[j + 1].c_str()), atof(tokens[j + 2].c_str())));
                    }
                } else if ( id.find("normals") != string::npos ) {
                    string normalString = source->FirstChildElement("float_array")->GetText();
                    istringstream iss(normalString);
                    vector<string> tokens;
                    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
                    for ( int j = 0; j < tokens.size(); j += 3 ) {
                        normals.push_back(DirectX::XMFLOAT3(atof(tokens[j].c_str()), atof(tokens[j + 1].c_str()), atof(tokens[j + 2].c_str())));
                    }
                } else if ( id.find("map-0") != string::npos ) {
                    string normalString = source->FirstChildElement("float_array")->GetText();
                    istringstream iss(normalString);
                    vector<string> tokens;
                    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
                    for ( int j = 0; j < tokens.size(); j += 2 ) {
                        tex0.push_back(DirectX::XMFLOAT2(atof(tokens[j].c_str()), atof(tokens[j + 1].c_str())));
                    }
                }
            }
        }
        string vertexString = geometry->FirstChildElement("mesh")->FirstChildElement("polylist")->FirstChildElement("p")->GetText();
        istringstream iss(vertexString);
        vector<string> tokens;
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));  
        vector<int> polyList;
        for ( auto it = tokens.begin(); it != tokens.end(); ++it ) {
            polyList.push_back(atoi((*it).c_str()));
        }
        createBuffers(positions, normals, tex0, polyList);
        loadSockets(mesh);
    } else {
        cout << "Failed loading mesh! " << mesh.ErrorID() << endl;
        return;
    }
}
void
Mesh::createBuffers(std::vector<DirectX::XMFLOAT3> pPositions, std::vector<DirectX::XMFLOAT3> pNormals, std::vector<DirectX::XMFLOAT2> pUV, std::vector<int> pPolyList) {
    using namespace std;
    vector<Vertex> vertices;
    vector<int> indices;
        
    for ( int j = 0; j < pPolyList.size(); j += 3 ) {
        Vertex v;
        v.Position = pPositions[pPolyList[j]];
        v.Normal = pNormals[pPolyList[j+1]];
        v.Tex0 = pUV[pPolyList[j+2]];
        vertices.push_back(v);
        indices.push_back(indices.size());
    }
        
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    vbd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = &vertices[0];
    HRESULT hr = mLoadingTools->getDevice()->CreateBuffer(&vbd, &vertexData, &mVertexBuffer);
    if ( FAILED(hr) ) {
        cout << "Failed creting vertex buffer!" << endl;
        return;
    }
    mIndexCount = indices.size();
    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(int) * mIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = &indices[0];
    hr = mLoadingTools->getDevice()->CreateBuffer(&ibd, &indexData, &mIndexBuffer);
    if ( FAILED(hr) ) {
        cout << "Failed creting index buffer!" << endl;
        return;
    }
}
void            
Mesh::loadSockets(const tinyxml2::XMLDocument& document) {
    using namespace tinyxml2;
    const XMLElement* socketsElement = document.FirstChildElement("COLLADA")->FirstChildElement("library_visual_scenes");
    if ( socketsElement->FirstChildElement("visual_scene") ) {
        const XMLElement* sceneElement = socketsElement->FirstChildElement("visual_scene");
        for ( const XMLElement* child = sceneElement->FirstChildElement("node"); child != NULL; child = child->NextSiblingElement("node") ) {
            if ( child->FirstChildElement("matrix") ) {
                mSockets.push_back(Socket(child->Attribute("id"), 
                    Matrix(child->FirstChildElement("matrix"))));
            }
        }
    }
}
std::vector<Socket>*        
Mesh::getSockets() {
    return &mSockets;
}
ID3D11Buffer*   
Mesh::getVertexBuffer() {
    return mVertexBuffer;
}
ID3D11Buffer*   
Mesh::getIndexBuffer() {
    return mIndexBuffer;
}
UINT
Mesh::getIndexCount() {
    return mIndexCount;
}
Mesh::~Mesh() {
    mVertexBuffer->Release();
    mIndexBuffer->Release();
}
