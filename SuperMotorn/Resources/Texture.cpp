#include "Texture.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include <iostream>
Texture::Texture(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools) :
AbstractResource(pFileName, pTimeStamp, pLoadingTools) {
}
void
Texture::load() {
    HRESULT hr;
    if ( mFileName.find(L".dds") != std::string::npos) {
        hr = DirectX::CreateDDSTextureFromFile(mLoadingTools->getDevice(), 
            mFileName.c_str(), 0, &mTextureView);
    } else {
        hr = DirectX::CreateWICTextureFromFile(mLoadingTools->getDevice(), mLoadingTools->getContext(),
            mFileName.c_str(), 0, &mTextureView, 0 );
    }
    if ( FAILED(hr) ) {
        std::wcout << "Failed loading texture " << mFileName << " because: " << hr << std::endl;
    }
}
ID3D11ShaderResourceView*   
Texture::getTextureView() {
    return mTextureView;
}
Texture::~Texture() {
    mTextureView->Release();
}
