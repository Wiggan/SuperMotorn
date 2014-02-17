#include "Renderer.h"
#include <DirectXMath.h>
#include "Vertex.h"
#include "Texture.h"
#include "Material.h"
#include "Shader.h"
#include <iostream>
Renderer::Renderer(HWND pWindow, int width, int height) : mD3DInit(new D3DInitializer(pWindow, width, height)), mWindow(pWindow),
    mProjectionTransform(DirectX::XMMatrixPerspectiveFovLH(0.25f*DirectX::XM_PI, (float)width / (float)height, 1.0f, 1000.0f)),
    mStride(sizeof(Vertex)) {
}
void
Renderer::setActiveCamera(BaseCamera* pCamera) {
    mCamera = pCamera;
}
void
Renderer::init() {
    mD3DInit->createDeviceAndContext();
    mD3DInit->createSwapChain();
    mD3DInit->createDepthStencil();
    mD3DInit->setRenderTarget();
    mD3DInit->createRasterizerStates();
    mD3DInit->createConstantBuffers();
    mD3DInit->setSamplerState();
    mDevice = mD3DInit->mDevice;
    mContext = mD3DInit->mContext;
}
ID3D11Device*
Renderer::getDevice() {
    return mDevice;
}
ID3D11DeviceContext*
Renderer::getContext() {
    return mContext;
}
void
Renderer::drawSolid(Mesh* pMesh, const Matrix& pWorldTransform, Material* pMaterial) {
    mRenderOrders.push_back(RenderOrder(pMesh, pWorldTransform, pMaterial));
}
void
Renderer::drawPointLight(const PointLight* pPointLight) {
    mPointLights.push_back(pPointLight);
}
void
Renderer::begin() {
    float color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    mContext->ClearRenderTargetView(mD3DInit->mRenderTargetView, color);
    mContext->ClearDepthStencilView(mD3DInit->mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    PerFrameConstants constants;
    constants.viewMatrix = mCamera->getViewTransform().transposed();
    constants.projectionMatrix = mProjectionTransform.transposed();
    constants.cameraPosition = mCamera->getPosition();
    constants.pointLightCount = mPointLights.size();
    for ( int i = 0; i < constants.pointLightCount; ++i ) {
        constants.pointLights[i] = *mPointLights[i];
    }
    setFrameConstants(constants);
}
void
Renderer::renderSolids() {
    mContext->RSSetState(mD3DInit->mRasterStateSolid);
    int orderCount = mRenderOrders.size();
    for ( int i = orderCount-1; i >= 0; i-- ) {
        drawMesh(mRenderOrders[i].transform, mRenderOrders[i].mesh, mRenderOrders[i].material);
    }
}
void 
Renderer::end() {
    mPointLights.clear();
    mRenderOrders.clear();
    mD3DInit->mSwapChain->Present(0, 0);
    //mDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_SUMMARY);
}
void
Renderer::drawMesh(const Matrix& pTransform, Mesh* pMesh, Material* pMaterial) {
    PerObjectConstants constants;
    mContext->VSSetShader(pMaterial->getShader()->getVertexShader(), 0, 0);
    mContext->PSSetShader(pMaterial->getShader()->getPixelShader(), 0, 0);
    mContext->IASetInputLayout(pMaterial->getShader()->getInputLayout());
    constants.worldMatrix   = pTransform.transposed();
    constants.ambient       = pMaterial->getAmbient();
    constants.diffuse       = pMaterial->getDiffuse();
    constants.specular      = pMaterial->getSpecular();
    constants.reflect       = pMaterial->getReflect();
    constants.color         = pMaterial->getColor();
    ID3D11Buffer* vb        = pMesh->getVertexBuffer();
    ID3D11Buffer* ib        = pMesh->getIndexBuffer();
    if ( pMaterial->getDiffuseMap() ) {
        ID3D11ShaderResourceView* texture   = pMaterial->getDiffuseMap()->getTextureView();
        mContext->PSSetShaderResources(1, 1, &texture);
        constants.useDiffuseMap = true;
    }
    if ( pMaterial->getSpecularMap() ) {
        ID3D11ShaderResourceView* texture   = pMaterial->getSpecularMap()->getTextureView();
        mContext->PSSetShaderResources(2, 1, &texture);
        constants.useSpecularMap = true;
    }
    if ( pMaterial->getGlossMap() ) {
        ID3D11ShaderResourceView* texture   = pMaterial->getGlossMap()->getTextureView();
        mContext->PSSetShaderResources(3, 1, &texture);
        constants.useGlossMap = true;
    }
    setObjectConstants(constants);
    UINT offset = 0;
    mContext->IASetVertexBuffers(0, 1, &vb, &mStride, &offset);
    mContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
    mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mContext->DrawIndexed(pMesh->getIndexCount(), 0, 0);
}
void
Renderer::setObjectConstants(const PerObjectConstants& pConstants) {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = mContext->Map(mD3DInit->mPerObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if ( FAILED(hr) ) {
        MessageBox(0, L"Map failed", 0, 0);
        exit(-1);
    }
    memcpy(mappedResource.pData, &pConstants, sizeof(pConstants));
    mContext->Unmap(mD3DInit->mPerObjectBuffer, 0);
    mContext->PSSetConstantBuffers(0, 1, &mD3DInit->mPerObjectBuffer);
    mContext->VSSetConstantBuffers(0, 1, &mD3DInit->mPerObjectBuffer);
}
void
Renderer::setFrameConstants(const PerFrameConstants& pConstants) {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = mContext->Map(mD3DInit->mPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if ( FAILED(hr) ) {
        MessageBox(0, L"Map failed", 0, 0);
        exit(-1);
    }
    memcpy(mappedResource.pData, &pConstants, sizeof(pConstants));
    mContext->Unmap(mD3DInit->mPerFrameBuffer, 0);
    mContext->PSSetConstantBuffers(1, 1, &mD3DInit->mPerFrameBuffer);
    mContext->VSSetConstantBuffers(1, 1, &mD3DInit->mPerFrameBuffer);
}
void
Renderer::setSeldomConstants(const SeldomConstants& pConstants) {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = mContext->Map(mD3DInit->mSeldomBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if ( FAILED(hr) ) {
        MessageBox(0, L"Map failed", 0, 0);
        exit(-1);
    }
    memcpy(mappedResource.pData, &pConstants, sizeof(pConstants));
    mContext->Unmap(mD3DInit->mSeldomBuffer, 0);
    mContext->PSSetConstantBuffers(2, 1, &mD3DInit->mSeldomBuffer);
    mContext->VSSetConstantBuffers(2, 1, &mD3DInit->mSeldomBuffer);
}
void
Renderer::setSkyBox(Texture* pSkyBox) {
    if ( pSkyBox ) {
        ID3D11ShaderResourceView* texture   = pSkyBox->getTextureView();
        mContext->PSSetShaderResources(0, 1, &texture);
    }
}
Renderer::~Renderer() {
    delete mD3DInit;
}