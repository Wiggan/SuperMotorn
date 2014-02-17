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

    void                    setObjectConstants(const PerObjectConstants& pConstants);
    void                    setFrameConstants(const PerFrameConstants& pConstants);
    void                    drawMesh(const Matrix& pTransform, Mesh* pMesh, Material* pMaterial);
public:
                            Renderer(HWND pWindow, int width, int height);
                            ~Renderer();
    ID3D11Device*           getDevice();
    ID3D11DeviceContext*    getContext();
    void                    setSeldomConstants(const SeldomConstants& pConstants);
    void                    setSkyBox(Texture* pSkyBox);
    void                    init();
    void                    setActiveCamera(BaseCamera* pCamera);
    void                    begin();
    void                    drawPointLight(const PointLight* pPointLight);
    void                    drawSolid(Mesh* pMesh, const Matrix& pWorldTransform, Material* pMaterial);
    void                    drawTransparent();
    void                    renderSolids();
    void                    renderTransparents();
    void                    end();
};

#endif