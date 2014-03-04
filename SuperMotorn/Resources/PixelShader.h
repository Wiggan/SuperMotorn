#pragma once
#include "AbstractResource.h"
class PixelShader :
    public AbstractResource {
    ID3D11Device*               mDevice;
    ID3D11DeviceContext*        mContext;
    ID3D11PixelShader*          mPixelShader;
public:
                            PixelShader(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
    virtual void            load();
    ID3D11PixelShader*      getPixelShader();   
                            ~PixelShader();
};

