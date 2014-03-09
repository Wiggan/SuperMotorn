#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include "DirectionalLight.h"
#include "PointLight.h"
struct PerObjectConstants {
    PerObjectConstants() {
        ZeroMemory(this, sizeof(*this));
    }
    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMFLOAT4   ambient;
    DirectX::XMFLOAT4   diffuse;
    DirectX::XMFLOAT4   specular;
    DirectX::XMFLOAT4   reflect;
    DirectX::XMFLOAT3   color;
    int                 useDiffuseMap;
    int                 useSpecularMap;
    int                 useGlossMap;
    int                 useGlowMap;
    float               pad;
};
struct PerFrameConstants {
    PerFrameConstants() {
        ZeroMemory(this, sizeof(*this));
    }
    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projectionMatrix;
    DirectX::XMFLOAT3   cameraPosition;
    int                 pointLightCount;
    PointLight          pointLights[10];
};
struct SeldomConstants {
    SeldomConstants() {
        ZeroMemory(this, sizeof(*this));
    }
    DirectionalLight directionalLight;
    DirectX::XMFLOAT4 fogColor;
    float fogStart;
    float fogRange;
    int useSkyColor;
    int pad;
};
class D3DInitializer {
public:
    HWND                        mWindow;
    int                         mWidth;
    int                         mHeight;
    UINT                        m4xMsaaQuality;
    ID3D11Device*               mDevice;
    ID3D11DeviceContext*        mContext;
    ID3D11SamplerState*         mSamplerState;
    ID3D11UnorderedAccessView*  mComputeUAV;
    ID3D11ShaderResourceView*   mComputeSRV;
    ID3D11UnorderedAccessView*  mBlurUAV;
    ID3D11ShaderResourceView*   mBlurSRV;
    ID3D11RenderTargetView*     mOffscreenRTV;
    ID3D11ShaderResourceView*   mOffscreenSRV;
    ID3D11RenderTargetView*     mGlowRTV;
    ID3D11ShaderResourceView*   mGlowSRV;
    ID3D11UnorderedAccessView*  mGlowUAV;
    ID3D11RenderTargetView*     mRenderTargetView;
    IDXGISwapChain*             mSwapChain;
    ID3D11DepthStencilView*     mDepthStencilView;
    ID3D11RasterizerState*      mRasterStateSolid;
#ifdef _DEBUG
    ID3D11Debug*                mDebug;
    ID3D11InfoQueue*            mInfoQueue;
#endif
    ID3D11Buffer*               mPerFrameBuffer;
    ID3D11Buffer*               mPerObjectBuffer;
    ID3D11Buffer*               mSeldomBuffer;

    void                        createSwapChain();
    void                        createDeviceAndContext();
    void                        createDepthStencil();
    void                        setRenderTarget();
    void                        loadShader();
    void                        createRasterizerStates();
    void                        createComputeTexture(ID3D11ShaderResourceView** pSRV, ID3D11UnorderedAccessView** pUAV);
    void                        createOffscreenTexture(ID3D11ShaderResourceView** pSRV, ID3D11RenderTargetView** pRTV);
    void                        createTexture(ID3D11ShaderResourceView** pSRV, ID3D11RenderTargetView** pRTV, ID3D11UnorderedAccessView** pUAV);
    void                        setSamplerState();
    void                        createConstantBuffers();
                                D3DInitializer(HWND pWindow, int width, int height);
                                ~D3DInitializer();
};

