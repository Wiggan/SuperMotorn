#pragma once
#include "AbstractResource.h"
class ComputeShader :
    public AbstractResource {
    ID3D11Device*               mDevice;
    ID3D11DeviceContext*        mContext;
    ID3D11ComputeShader*        mComputeShader;
public:
                            ComputeShader(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
    virtual void            load();
    ID3D11ComputeShader*    getComputeShader();  
                            ~ComputeShader();
};

