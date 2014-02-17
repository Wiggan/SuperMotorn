#pragma once
#include "AbstractResource.h"
class Texture :
    public AbstractResource {
    ID3D11Device*               mDevice;
    ID3D11DeviceContext*        mContext;
    ID3D11ShaderResourceView*   mTextureView;
public:
    Texture(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
    virtual void                load();
    ID3D11ShaderResourceView*   getTextureView();
    ~Texture();
};

