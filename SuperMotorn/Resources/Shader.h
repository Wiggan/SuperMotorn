#pragma once
#include "AbstractResource.h"
#include <d3d11.h>
class Shader : public AbstractResource {
    ID3D11Device*               mDevice;
    ID3D11DeviceContext*        mContext;
    ID3D11VertexShader*         mVertexShader;
    ID3D11PixelShader*          mPixelShader;
    ID3D11InputLayout*          mInputLayout;
public:
                            Shader(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
    virtual void            load();
    ID3D11VertexShader*     getVertexShader();
    ID3D11PixelShader*      getPixelShader();                        
    ID3D11InputLayout*      getInputLayout();
                            ~Shader();

};
