#pragma once
#include <string>
#include "TimeStamp.h"
#include <d3d11.h>
class AbstractResource;
class LoadingToolsInterface {
public:
    virtual ID3D11Device*               getDevice() = 0;
    virtual ID3D11DeviceContext*        getContext() = 0;
    virtual AbstractResource*           getTexture(const std::wstring &pFileName) = 0;
    virtual AbstractResource*           getVertexShader(const std::wstring &pFileName) = 0;
    virtual AbstractResource*           getPixelShader(const std::wstring &pFileName) = 0;
    virtual AbstractResource*           getComputeShader(const std::wstring &pFileName) = 0;
};
class AbstractResource {
protected:
    std::wstring            mFileName;
    TimeStamp               mTimeStamp;
    LoadingToolsInterface*  mLoadingTools;
public:
                        AbstractResource(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
    virtual void        load() = 0;
    TimeStamp&          getTimeStamp();
    void                setTimeStamp(TimeStamp& pTimeStamp);
    const std::wstring& getFileName();
    virtual             ~AbstractResource();
};

