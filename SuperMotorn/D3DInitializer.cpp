#include "D3DInitializer.h"
#include <fstream>
#include <iostream>
#include <d3d11sdklayers.h>

D3DInitializer::D3DInitializer(HWND pWindow, int width, int height) : mWindow(pWindow), mWidth(width), mHeight(height) {
}
void
D3DInitializer::createDepthStencil() {
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.ArraySize          = 1;
    depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.Width              = mWidth;
    depthStencilDesc.Height             = mHeight;
    depthStencilDesc.MipLevels          = 1;
    depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count   = 4;
    depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
    depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
    depthStencilDesc.CPUAccessFlags     = 0;
    depthStencilDesc.MiscFlags          = 0;
    ID3D11Texture2D* depthStencilBuffer;
    HRESULT hr = mDevice->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateTexture2D failed!", 0, 0);
        return;
    }
    hr = mDevice->CreateDepthStencilView(depthStencilBuffer, 0, &mDepthStencilView);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateDepthStencilView failed!", 0, 0);
        return;
    }
    depthStencilBuffer->Release();
}
void
D3DInitializer::setRenderTarget() {
    mContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
    D3D11_VIEWPORT vp;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Height   = (float)mHeight;
    vp.Width    = (float)mWidth;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f; 
    mContext->RSSetViewports(1, &vp);
}
void
D3DInitializer::createDeviceAndContext() {
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED,
        0, 0,
        D3D11_SDK_VERSION,
        &mDevice,
        &featureLevel,
        &mContext);
    if ( FAILED(hr) ) {
        MessageBox(0, L"Direct3DCreateDevice failed!", 0, 0);
        return;
    }
    if ( featureLevel != D3D_FEATURE_LEVEL_11_0 ) {
        MessageBox(0, L"Direct3D Feature Level 11 unsupported!", 0, 0);
        return;
    }
    mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&mDebug));
    mDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_SUMMARY);
    
    mDevice->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&mInfoQueue));
    mInfoQueue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_CORRUPTION, true );
    mInfoQueue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_ERROR, true );
}
void
D3DInitializer::createSwapChain() {
    mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width                     = mWidth;
    sd.BufferDesc.Height                    = mHeight;
    sd.BufferDesc.RefreshRate.Numerator     = 60;
    sd.BufferDesc.RefreshRate.Denominator   = 1;
    sd.BufferDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering          = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling                   = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count                     = 4;
    sd.SampleDesc.Quality                   = m4xMsaaQuality - 1;
    sd.BufferUsage                          = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount                          = 1;
    sd.OutputWindow                         = mWindow;
    sd.Windowed                             = true;
    sd.SwapEffect                           = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags                                = 0;
    IDXGIDevice* dxgiDevice = 0;
    HRESULT hr = mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    if ( FAILED(hr) ) {
        MessageBox(0, L"QueryInterface failed", 0, 0);
        return;
    }
    IDXGIAdapter* dxgiAdapter = 0;
    hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
    if ( FAILED(hr) ) {
        MessageBox(0, L"Failed finding IDXGIAdapter", 0, 0);
        return;
    }
    IDXGIFactory* dxgiFactory = 0;
    hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
    if ( FAILED(hr) ) {
        MessageBox(0, L"Failed finding IDXGIFactory", 0, 0);
        return;
    }
    hr = dxgiFactory->CreateSwapChain(mDevice, &sd, &mSwapChain);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateSwapChain failed", 0, 0);
        return;
    }
    dxgiDevice->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();
    ID3D11Texture2D* backBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    mDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
    backBuffer->Release();
}
void
D3DInitializer::createRasterizerStates() {
    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.CullMode = D3D11_CULL_NONE;
    rd.FillMode = D3D11_FILL_SOLID;
    HRESULT hr = mDevice->CreateRasterizerState(&rd, &mRasterStateSolid);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateRasterizerState failed", 0, 0);
    }
}
void                        
D3DInitializer::setSamplerState() {
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.AddressU         = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV         = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW         = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.Filter           = D3D11_FILTER_ANISOTROPIC;
    sd.MaxAnisotropy    = 4;
    HRESULT hr = mDevice->CreateSamplerState(&sd, &mSamplerState);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateSamplerState failed", 0, 0);
    }
    mContext->PSSetSamplers(0, 1, &mSamplerState);
}
void
D3DInitializer::createConstantBuffers() {
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(PerFrameConstants);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    HRESULT hr = mDevice->CreateBuffer(&bd, NULL, &mPerFrameBuffer);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateBuffer failed", 0, 0);
        exit(-1);
    }
    bd.ByteWidth = sizeof(PerObjectConstants);
    hr = mDevice->CreateBuffer(&bd, NULL, &mPerObjectBuffer);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateBuffer failed", 0, 0);
        exit(-1);
    }
    bd.ByteWidth = sizeof(SeldomConstants);
    hr = mDevice->CreateBuffer(&bd, NULL, &mSeldomBuffer);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateBuffer failed", 0, 0);
        exit(-1);
    }
}
D3DInitializer::~D3DInitializer() {
    mRenderTargetView->Release();
    mSwapChain->Release();
    mDepthStencilView->Release();
    mRasterStateSolid->Release();
    mDevice->Release();
    mContext->Release();
    mPerFrameBuffer->Release();
    mPerObjectBuffer->Release();
    mSeldomBuffer->Release();
    mSamplerState->Release();
    mInfoQueue->Release();
    mDebug->Release();
}
