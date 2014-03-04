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
    DirectX::XMFLOAT2   pad;
};
struct PerFrameConstants {
    PerFrameConstants() {
        ZeroMemory(this, sizeof(*this));
    }
    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projectionMatrix;
    DirectX::XMFLOAT3   cameraPosition;
    int                 pointLightCount;
    PointLight          pointLights[3];
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
    ID3D11UnorderedAccessView*  mComputeUnorderedAccessView;
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
    void                        createComputeTexture();
    void                        setSamplerState();
    void                        createConstantBuffers();
                                D3DInitializer(HWND pWindow, int width, int height);
                                ~D3DInitializer();
};

