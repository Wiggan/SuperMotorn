#ifndef MESH_H
#define MESH_H
#include <d3d11.h>
#include <DirectXMath.h>
#include "AbstractResource.h"
#include <vector>
#include "Socket.h"
class Texture;
class Material;
namespace tinyxml2 {
    class XMLDocument;
    class XMLElement;
}
class Mesh : public AbstractResource {
private:
    ID3D11Buffer*           mVertexBuffer;
    ID3D11Buffer*           mIndexBuffer;
    UINT                    mIndexCount;
    std::vector<Socket>     mSockets;

    void                    createBuffers(std::vector<DirectX::XMFLOAT3> pPositions, std::vector<DirectX::XMFLOAT3> pNormals, std::vector<DirectX::XMFLOAT2> pUV, std::vector<int> pPolyList);
    void                    loadSockets(const tinyxml2::XMLDocument& document);
public:
                                Mesh(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
                                Mesh(std::vector<DirectX::XMFLOAT3> pPositions, std::vector<DirectX::XMFLOAT3> pNormals, std::vector<DirectX::XMFLOAT2> pUV, std::vector<int> pPolyList, LoadingToolsInterface* pLoadingTools);
    ID3D11Buffer*               getVertexBuffer();
    ID3D11Buffer*               getIndexBuffer();
    UINT                        getIndexCount();
    std::vector<Socket>*        getSockets();
    virtual void                load();
                                ~Mesh();
};

#endif