#ifndef RENDERER_H
#define RENDERER_H
#include <d3d11.h>
#include "Matrix.h"
#include "BaseCamera.h"
#include "Mesh.h"
#include <vector>
#include "D3DInitializer.h"
#include "PointLight.h"
#include "Material.h"
#include "ResourceLoader.h"
class MeshComponent;
class ComputeShader;
class VertexShader;
class PixelShader;
enum TextureSlots {
    CUBE_MAP = 0,
    DIFFUSE_MAP = 1,
    SPECULAR_MAP = 2,
    GLOSS_MAP = 3,
    GLOW_MAP = 4,
    OFFSCREEN1 = 5,
    OFFSCREEN2 = 6,
    COMPUTE_RESULT = 7,

};
struct RenderOrder {
    RenderOrder(Mesh* pMesh, const Matrix& pWorldTransform, Material* pMaterial) : mesh(pMesh), transform(pWorldTransform), material(pMaterial) {}
    Mesh*       mesh;
    Matrix      transform;
    Material*   material;
};
class Renderer {
private:
    D3DInitializer*                 mD3DInit;
    ID3D11Device*                   mDevice;
    ID3D11DeviceContext*            mContext;
    UINT                            mStride;
    BaseCamera*                     mCamera;
    HWND                            mWindow;
    Matrix                          mProjectionTransform;
    std::vector<RenderOrder>        mRenderOrders;
    std::vector<const PointLight*>  mPointLights;
    MeshComponent*                  mSkyMesh;
    Material*                       mSkyMaterial;
    ComputeShader*                  mBlurHorizontalComputeShader;
    ComputeShader*                  mBlurVerticalComputeShader;
    ComputeShader*                  mSumComputeShader;
    VertexShader*                   mPresentVertexShader;
    PixelShader*                    mPlainPixelShader;
    Mesh*                           mPresentMesh;

    void                    setObjectConstants(const PerObjectConstants& pConstants);
    void                    setFrameConstants(const PerFrameConstants& pConstants);
    void                    drawMesh(const Matrix& pTransform, Mesh* pMesh, Material* pMaterial);
    void                    drawGlow(const Matrix& pTransform, Mesh* pMesh, Material* pMaterial);
public:
                            Renderer(HWND pWindow, int width, int height);
                            ~Renderer();
    ID3D11Device*           getDevice();
    ID3D11DeviceContext*    getContext();
    void                    setSeldomConstants(const SeldomConstants& pConstants);
    void                    setSkyBox(Material* pMaterial, MeshComponent* pMesh);
    void                    init(ResourceLoader* pResourceLoader);
    void                    setActiveCamera(BaseCamera* pCamera);
    void                    begin();
    void                    renderSolids();
    void                    renderTransparents();
    void                    renderToBackBuffer();
    void                    blur(ID3D11ShaderResourceView* pSRV, ID3D11UnorderedAccessView* pUAV, int blurCount);
    void                    end();
    void                    drawPointLight(const PointLight* pPointLight);
    void                    drawSolid(Mesh* pMesh, const Matrix& pWorldTransform, Material* pMaterial);
    void                    drawTransparent();
};

#endif