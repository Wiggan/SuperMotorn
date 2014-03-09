#include "Renderer.h"
#include <DirectXMath.h>
#include "Vertex.h"
#include "Texture.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include <iostream>
#include "MeshComponent.h"
Renderer::Renderer(HWND pWindow, int width, int height) : mD3DInit(new D3DInitializer(pWindow, width, height)), mWindow(pWindow),
    mProjectionTransform(DirectX::XMMatrixPerspectiveFovLH(0.25f*DirectX::XM_PI, (float)width / (float)height, 1.0f, 10000.0f)),
    mStride(sizeof(Vertex)) {
}
void
Renderer::setActiveCamera(BaseCamera* pCamera) {
    mCamera = pCamera;
}
void
Renderer::init(ResourceLoader* pResourceLoader) {
    mD3DInit->createDeviceAndContext();
    mD3DInit->createSwapChain();
    mD3DInit->createDepthStencil();
    mD3DInit->setRenderTarget();
    mD3DInit->createRasterizerStates();
    mD3DInit->createConstantBuffers();
    mD3DInit->setSamplerState();
    mD3DInit->createComputeTexture(&mD3DInit->mComputeSRV, &mD3DInit->mComputeUAV);
    mD3DInit->createComputeTexture(&mD3DInit->mBlurSRV, &mD3DInit->mBlurUAV);
    mD3DInit->createTexture(&mD3DInit->mOffscreenSRV, &mD3DInit->mOffscreenRTV, NULL);
    mD3DInit->createTexture(&mD3DInit->mGlowSRV, &mD3DInit->mGlowRTV, &mD3DInit->mGlowUAV);
    mDevice = mD3DInit->mDevice;
    mContext = mD3DInit->mContext;
    pResourceLoader->init(mDevice, mContext);
    mBlurHorizontalComputeShader = (ComputeShader*)pResourceLoader->getComputeShader(L"blurHorizontalCompute.fx");
    mBlurVerticalComputeShader = (ComputeShader*)pResourceLoader->getComputeShader(L"blurVerticalCompute.fx");
    mSumComputeShader = (ComputeShader*)pResourceLoader->getComputeShader(L"sumCompute.fx");
    mPresentVertexShader = (VertexShader*)pResourceLoader->getVertexShader(L"presentVertex.fx");
    mPlainPixelShader = (PixelShader*)pResourceLoader->getPixelShader(L"plainPixel.fx");
    mPresentMesh = (Mesh*)pResourceLoader->getResource<Mesh>(L"offscreen.dae");
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
    float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    ID3D11RenderTargetView* renderTargets[2] = { mD3DInit->mOffscreenRTV, mD3DInit->mGlowRTV };
    mContext->OMSetRenderTargets(2, renderTargets, mD3DInit->mDepthStencilView);
    mContext->ClearRenderTargetView(mD3DInit->mOffscreenRTV, color);
    mContext->ClearRenderTargetView(mD3DInit->mGlowRTV, color);
    mContext->ClearDepthStencilView(mD3DInit->mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
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
    drawMesh(Matrix(Vector3(300.0f, 300.0f, 300.0f), Vector3(0,0,0), mCamera->getPosition()), mSkyMesh->getMesh(), mSkyMaterial);
}
void
Renderer::blur(ID3D11ShaderResourceView* pSRV, ID3D11UnorderedAccessView* pUAV, int blurCount) {
    ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
    ID3D11ShaderResourceView* ppSRVNULL[1] = { NULL };
    for ( int i = 0; i < blurCount; i++ ) {
        mContext->CSSetShader(mBlurHorizontalComputeShader->getComputeShader(), 0, 0);
        mContext->CSSetShaderResources( OFFSCREEN2, 1, &pSRV);
        mContext->CSSetUnorderedAccessViews(0, 1, &mD3DInit->mBlurUAV, NULL);
        mContext->Dispatch(48,32,1);
        mContext->CSSetUnorderedAccessViews( 0, 1, ppUAViewNULL, NULL );
        mContext->CSSetShaderResources( OFFSCREEN2, 1, ppSRVNULL );  
        mContext->CSSetShader(mBlurVerticalComputeShader->getComputeShader(), 0, 0);
        mContext->CSSetShaderResources( OFFSCREEN2, 1, &mD3DInit->mBlurSRV);
        mContext->CSSetUnorderedAccessViews(0, 1, &pUAV, NULL);
        mContext->Dispatch(48,32,1);
        mContext->CSSetUnorderedAccessViews( 0, 1, ppUAViewNULL, NULL );
        mContext->CSSetShaderResources( OFFSCREEN2, 1, ppSRVNULL );  
    }
    mContext->CSSetShader(0, 0, 0);
}
void
Renderer::renderTransparents() {
}
void                    
Renderer::renderToBackBuffer() {
    ID3D11RenderTargetView* ppRTVNULL[2] = { NULL, NULL };
    mContext->OMSetRenderTargets(2, ppRTVNULL, mD3DInit->mDepthStencilView);
    float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    blur(mD3DInit->mGlowSRV, mD3DInit->mGlowUAV, 2);
    mContext->OMSetRenderTargets(1, &mD3DInit->mRenderTargetView, mD3DInit->mDepthStencilView);
    mContext->ClearRenderTargetView(mD3DInit->mRenderTargetView, color);
    mContext->ClearDepthStencilView(mD3DInit->mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
    mContext->CSSetShader(mSumComputeShader->getComputeShader(), 0, 0);
    mContext->CSSetShaderResources( OFFSCREEN1, 1, &mD3DInit->mOffscreenSRV);
    mContext->CSSetShaderResources( OFFSCREEN2, 1, &mD3DInit->mGlowSRV);
    mContext->CSSetUnorderedAccessViews(0, 1, &mD3DInit->mComputeUAV, NULL);
    mContext->Dispatch(48,32,1);
    ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
    ID3D11ShaderResourceView* ppSRVNULL[1] = { NULL };
    mContext->CSSetUnorderedAccessViews( 0, 1, ppUAViewNULL, NULL );
    mContext->CSSetShaderResources( OFFSCREEN1, 1, ppSRVNULL );  
    mContext->CSSetShaderResources( OFFSCREEN2, 1, ppSRVNULL );  
    mContext->CSSetShader(0, 0, 0);
    mContext->VSSetShader(mPresentVertexShader->getVertexShader(), 0, 0);
    mContext->PSSetShader(mPlainPixelShader->getPixelShader(), 0, 0);
    mContext->IASetInputLayout(mPresentVertexShader->getInputLayout());
    ID3D11Buffer* vb        = mPresentMesh->getVertexBuffer();
    ID3D11Buffer* ib        = mPresentMesh->getIndexBuffer();
    mContext->PSSetShaderResources(COMPUTE_RESULT, 1, &mD3DInit->mComputeSRV);
    UINT offset = 0;
    mContext->IASetVertexBuffers(0, 1, &vb, &mStride, &offset);
    mContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
    mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mContext->DrawIndexed(mPresentMesh->getIndexCount(), 0, 0);
    mContext->PSSetShaderResources( COMPUTE_RESULT, 1, ppSRVNULL );  
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
    mContext->VSSetShader(pMaterial->getVertexShader()->getVertexShader(), 0, 0);
    mContext->PSSetShader(pMaterial->getPixelShader()->getPixelShader(), 0, 0);
    mContext->IASetInputLayout(pMaterial->getVertexShader()->getInputLayout());
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
        mContext->PSSetShaderResources(DIFFUSE_MAP, 1, &texture);
        constants.useDiffuseMap = true;
    }
    if ( pMaterial->getSpecularMap() ) {
        ID3D11ShaderResourceView* texture   = pMaterial->getSpecularMap()->getTextureView();
        mContext->PSSetShaderResources(SPECULAR_MAP, 1, &texture);
        constants.useSpecularMap = true;
    }
    if ( pMaterial->getGlossMap() ) {
        ID3D11ShaderResourceView* texture   = pMaterial->getGlossMap()->getTextureView();
        mContext->PSSetShaderResources(GLOSS_MAP, 1, &texture);
        constants.useGlossMap = true;
    }
    if ( pMaterial->getGlowMap() ) {
        ID3D11ShaderResourceView* texture   = pMaterial->getGlowMap()->getTextureView();
        mContext->PSSetShaderResources(GLOW_MAP, 1, &texture);
        constants.useGlowMap = true;
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
Renderer::setSkyBox(Material* pMaterial, MeshComponent* pMesh) {
    if ( pMaterial ) {
        ID3D11ShaderResourceView* texture   = pMaterial->getDiffuseMap()->getTextureView();
        mContext->PSSetShaderResources(CUBE_MAP, 1, &texture);
        mSkyMaterial = pMaterial;
        mSkyMesh = pMesh;
        mSkyMesh->setScale(Vector3(300.0f, 300.0f, 300.0f));
    }
}
Renderer::~Renderer() {
    delete mD3DInit;
}